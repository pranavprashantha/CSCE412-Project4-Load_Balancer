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


  requestQueue.push(r);
}