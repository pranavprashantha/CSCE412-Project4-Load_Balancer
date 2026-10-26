#ifndef FIREWALL_H
#define FIREWALL_H

#include <vector>
#include <string>
#include <sstream>

long long ipToNum(const std::string& ip);

class Firewall {
  private:
    std::vector<std::vector<long long>> ranges;

  public:
    bool blocked(const std::string& ipToCheck);
    void addToBlocked(const std::string& lowerIP, const std::string& upperIP);
};

#endif
