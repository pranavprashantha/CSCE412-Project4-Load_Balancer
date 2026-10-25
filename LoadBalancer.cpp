#include "LoadBalancer.h"

const std::string GREEN  = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED    = "\033[31m";
const std::string BLUE   = "\033[34m";
const std::string RESET  = "\033[0m";
const int STATUS_EVERY = 5000;

LoadBalancer::LoadBalancer(int servers, int cycle, bool verbose, ostream& log) : clock(0), verbose(verbose), scaleCycle(0), scaleN(cycle), log(log) {
  serverStartCount = servers;
  maxServers = servers;

  for(int i = 0 ; i < servers; i++) {
    servers.push_back(Webserver())
  }
} 

LoadBalancer::addRequest(Request& r) {
  bool blocked = firewall.blocked(r.ipIN);

  if(!blocked) {
    requestQueue.push(r);
    requestsPassed++;
    queueMaxCount = std::max(queueMaxCount, requestQueue.size());

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

LoadBalancer::cycle() {
  if (clock == 0) {
    queueStartCount = requestQueue.size();
  }

  for (auto it = servers.begin(); it != servers.end(); ) {
      if (it->isFree() && !it->isAborting() && !requestQueue.empty()) {
          it->assign(requestQueue.front());
          qrequestQueueueue.pop();
      }
      if (it->processCycle()) {
          requestsFinished++;
      }
      if (it->isAborting() && it->isFree()) {
          logEvent(YELLOW, "Removed server " + std::to_string(it->getId()));
          serversRemoved++;
          it = servers.erase(it);
      } else {
          ++it;
      }
  }

  if(clock - scaleCycle >= scaleN) {

    if(requestQueue.size() > 80 * getServerCount()) {
      servers.push_back(Webserver());
      serversAdded++;
      maxServers = std::max(maxServers, getServerCount());
      logEvent(GREEN, "Added server " + std::to_string(servers.back().getID) + "(new queue size: " + std::to_string(requestQueue.size()) + ")");
      scaleCycle = clock;
    }
    else if(requestQueue.size() < 50 * getServerCount()) {
      if(getServerCount() > 1) {
        int i = servers.size() -1;

        while(i >= 0  && servers[i].isAborting()) {
          i--;
        }

        if(i >=0) {
          servers[i].startAbort();
          logEvent(YELLOW, "Server " + std::to_string(servers[i].getId()) + " killed (new queue size: " + std::to_string(requestQueue.size()) + ")");
        }
        scaleCycle = clock;

      }


    }

  }

  clock++;
  if (clock % STATUS_EVERY == 0) {
    logEvent(BLUE, "Status: queue " + std::to_string(requestQueue.size()) + ", servers " + std::to_string(getServerCount()));
  }


}

LoadBalancer::addFirewallRule(std::string min, std::string max) {
  firewall.addToBlocked(min, max);
}