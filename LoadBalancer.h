#idndef "LOADBALANCER_H"
#define "LOADBALANCER_H"

#include "Request.h"
#include "Webserver.h"
#include "Firewall.h"

#include <vector>
#include <ostream>
#include <queue>
#include <string>
#include <iostream>

class LoadBalancer {
  private:

    int clock;
    int totalCycles;
    bool verbose;
    int scaleCycle;
    int scaleN;

    std::queue<Request> requestQueue;
    std::vector<Webserver> servers;
    std::ostream& log;
    Firewall firewall;

    //Log vars

    int requestsPassed = 0;
    int requestsBlocked = 0;
    int requestsFinished = 0;
    int pRequests = 0;
    int sRequests = 0;

    int serverStartCount = 0;
    int maxServers = 0;
    int serversAdded = 0;
    int serversRemoved = 0;

    int queueStartCount = 0;
    int queueMaxCount = 0;

    int getServerCount();
    void logEvent(std::string& color, std::string& message);
    

  public:
    LoadBalancer(int numServers, int scaleCycleNum, bool verbose, std::ostream& log);
    bool addRequest(Request& r);
    void addFirewall(std::string min, std::string max);
    void cycle();
    void summaryStart();
    void endSummary();
}

#endif