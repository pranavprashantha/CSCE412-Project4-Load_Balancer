#include "LoadBalancer.h"

const std::string GREEN  = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED    = "\033[31m";
const std::string BLUE   = "\033[34m";
const std::string RESET  = "\033[0m";
const int STATUS_EVERY = 5000;

LoadBalancer::LoadBalancer(int numServers, int cycle, bool verbose, std::ostream& log, int totalCycles) : clock(0), totalCycles(totalCycles), verbose(verbose), scaleCycle(0), scaleN(cycle), log(log) {
  serverStartCount = numServers;
  maxServers = numServers;

  for(int i = 0 ; i < numServers; i++) {
    servers.push_back(Webserver());
  }
}

bool LoadBalancer::addRequest(Request& r) {
  bool blocked = firewall.blocked(r.ipIN);

  if(!blocked) {
    requestQueue.push(r);
    requestsPassed++;
    queueMaxCount = std::max(queueMaxCount, (int)requestQueue.size());

    if(r.jobType == 'P') {
      pRequests++;
    } else {
      sRequests++;
    }

    return true;
  }

  requestsBlocked++;
  logEvent(RED, "Blocked IP " + r.ipIN);

  return false;


}

void LoadBalancer::cycle() {
  if (clock == 0) {
    queueStartCount = requestQueue.size();
    summaryStart();
  }

  for (auto it = servers.begin(); it != servers.end(); ) {
      if (it->isFree() && !it->isAborting() && !requestQueue.empty()) {
          it->assign(requestQueue.front());
          requestQueue.pop();
      }
      if (it->runCycle()) {
          requestsFinished++;
      }
      if (it->isAborting() && it->isFree()) {
          logEvent(YELLOW, "Removed server " + std::to_string(it->getID()));
          serversRemoved++;
          it = servers.erase(it);
      } else {
          ++it;
      }
  }

  if(clock - scaleCycle >= scaleN) {

    if((int)requestQueue.size() > 80 * getServerCount()) {
      servers.push_back(Webserver());
      serversAdded++;
      maxServers = std::max(maxServers, getServerCount());
      logEvent(GREEN, "Added server " + std::to_string(servers.back().getID()) + " (new queue size: " + std::to_string(requestQueue.size()) + ")");
      scaleCycle = clock;
    }
    else if((int)requestQueue.size() < 50 * getServerCount()) {
      if(getServerCount() > 1) {
        int i = servers.size() -1;

        while(i >= 0  && servers[i].isAborting()) {
          i--;
        }

        if(i >=0) {
          servers[i].startAbort();
          logEvent(YELLOW, "Server " + std::to_string(servers[i].getID()) + " killed (new queue size: " + std::to_string(requestQueue.size()) + ")");
        }
        scaleCycle = clock;

      }


    }

  }

  clock++;
  if (clock % STATUS_EVERY == 0) {
    logEvent(BLUE, "Status: queue " + std::to_string(requestQueue.size()) + ", servers " + std::to_string(getServerCount()));
  }

  if(clock == totalCycles) {
    endSummary();
  }
}

int LoadBalancer::getServerCount() {
  int total = 0;
  for(Webserver& server : servers) {
    if(!server.isAborting()) {
      total++;
    }
  }

  return total;

}

void LoadBalancer::addFirewall(std::string min, std::string max) {
  firewall.addToBlocked(min, max);
}

void LoadBalancer::logEvent(const std::string& color, const std::string& message) {
    if (!verbose) {
        return;
    }
    std::string line = "[Cycle " + std::to_string(clock) + "] " + message;
    std::cout << color << line << RESET << "\n";
    log << line << "\n";
}

void LoadBalancer::summaryStart() {
    std::ostringstream out;
    out << "========= Start Summary =========\n"
        << "Servers:            " << getServerCount() << "\n"
        << "Queue size:         " << requestQueue.size() << "\n"
        << "Scale wait (n):     " << scaleN << " cycles\n"
        << "Requests blocked:   " << requestsBlocked << "\n"
        << "=================================\n";
    std::cout << out.str();
    log << out.str();
}

void LoadBalancer::endSummary() {
    std::ostringstream out;
    out << "========== End Summary ==========\n"
        << "Total cycles:       " << clock << "\n"
        << "Requests generated: " << requestsPassed + requestsBlocked << "\n"
        << "Requests passed:    " << requestsPassed
        << " (P: " << pRequests << ", S: " << sRequests << ")\n"
        << "Requests blocked:   " << requestsBlocked << "\n"
        << "Requests finished:  " << requestsFinished << "\n"
        << "Queue size:         start " << queueStartCount << ", end " << requestQueue.size()
        << ", largest " << queueMaxCount << "\n"
        << "Servers:            start " << serverStartCount << ", end " << getServerCount()
        << ", highest " << maxServers << "\n"
        << "Servers added:      " << serversAdded << "\n"
        << "Servers removed:    " << serversRemoved << "\n"
        << "=================================\n";
    std::cout << out.str();
    log << out.str();
}

