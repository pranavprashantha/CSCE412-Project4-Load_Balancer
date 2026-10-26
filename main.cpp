#include "LoadBalancer.h"
#include "Request.h"

#include <fstream>
#include <iostream>
#include <map>
#include <string>

class main() {
  int arriveMin = 5; 
  int arriceMax = 20;
  int processMin = 10;
  int processMax = 100;
  int scaleWait = 50;
  bool verbose = true;

  int totalServer;
  int totalCycle;

  std::string blocked = "1.0.0.0-1.255.255.255";

  int mid = blocked.find('-');
  int blockedStart = blocked.substr(0, mid);
  int blockedEnd = blocked.substr(mid + 1);

  int nextRequestTime = 10;

  std::cout << "Enter the number of starting server: ";
  std::cin >> totalServer;
  std::cout << "Enter the number of total clock cycles: ";
  std::cin >> totalCycle;

  std::ifstream config("config.txt");
  std::ofstream logfile('log.txt');

  std::string line;
  while (config >> line) {

      size_t eq = line.find('=');
      if (eq == std::string::npos) continue;
      std::string key = line.substr(0, eq);
      std::string value = line.substr(eq + 1);

      if (key == "arrival_min") 
        arriveMin = std::stoi(value);
      else if (key == "arrival_max") 
        arriveMax = std::stoi(value);
      else if (key == "process_min") 
        processMin = std::stoi(value);
      else if (key == "process_max") 
        processMax = std::stoi(value);
      else if (key == "scale_wait") 
        scaleWait = std::stoi(value);
      else if (key == "blocked_range") 
        blockedRange = value;
      else if (key == "verbose") 
        verbose = (value == "true");
      else if (key == "log_file") 
        logFileName = value;


  }

  LoadBalancer lb(totalServer, scaleWait, verbose, logfile, totalCycle);
  lb.addFirewall(blockedStart, blockedEnd);

  for(int i = 0; i < totalServer * 100; i++) {
    Request tempRequest;
    tempRequest.randomize(processMin);
    lb.addRequest(tempRequest);
  }


  for(int i = 0; i < totalCycle; i++) {
    
    if(nextRequestTime-=1 ==  0) {
      Request tempRequest;
      tempRequest.randomize(processMin);
      lb.addRequest(tempRequest);
      
      nextRequestTime = randomInt(arriveMin. arriveMax)
    }

    lb.cycle();

  }

  return 0;


}