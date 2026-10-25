#idndef "LOADBALANCER_H"
#define "LOADBALANCER_H"

#include "Request.h"
#include "WebServer.h"
#include "Firewall.h"

#include <vector>
#include <ostream>
#include <queue>
#include <string>
#include <iostream>

class LoadBalancer {
  private:

    int clock;
    bool verbose;
    int scaleCycle;
    int scaleN;

    std::queue<Request> requestQueue;
    std::vector<Webserver> servers;
    std::ostream& log;
    Firewall firewall;

    //Log vars

    int requestsPassed;
    int requestsBlocked;
    int pRequests;
    int sRequests;

    int serverStartCount;
    int maxServers;
    int serversAdded;
    int serversRemoved;

    int queueStartCount;
    int queueMaxCount;

    int getServerCount;
    void logEvent(std::string& color, std::string& message);
    

  public:
    LoadBalancer(int numServers, int scaleCycleNum, bool verbose, std::ostream& log);
    bool addRequest(Request& r);
    void addFirewallRule(std::string min, std::string max);
    void runCycle();
    void summaryStart();
    void endSummary();
}

#endif