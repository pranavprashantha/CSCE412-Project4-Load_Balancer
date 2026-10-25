#ifndef FIREWALL_H
#define FIREWALL_H

#include <vector>
#include <string>
#include <sstream>

long long ipToNum(std::string& ip);

class Firewall {
  private:
    std::vector<vector<long long>> ranges;

  public:
    bool blocked(std::string& ipToCheck);
    void addToBlocked(std::string& lowerIP, std::string& upperIP);
}

#endif
