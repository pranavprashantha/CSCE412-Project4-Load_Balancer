#ifndef FIREWALL_H
#define FIREWALL_H

#include <vector>
#include <string>
#include <sstream>

unsigned int ipToNum(std::string& ip);

class Firewall {
  private:
    std::vector<vector<int>> ranges;

  public:
    bool blocked(std::string& ipToCheck);
    void addToBlocked(std::string& lowerIP, std::string& upperIP);
}

#endif
