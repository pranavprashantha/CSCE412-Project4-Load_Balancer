#include "Firewall.h"

long long Firewall::ipToNum(std::string& ip) {
  std::istringstream in(ip);
  long long a, b, c, d;
  char dot;
  in >> a >> dot >> b >> dot >> c >> dot >> d;
  return (a << 24) | (b << 16) | (c << 8) | d;
}

void Firewall::addToBlocked(std::string& lowerIP, std::string& upperIP) {
  vector<long long> temp = {ipToNum(lowerIP), ipToNum(upperIP)};
  ranges.push_back(temp);
}

bool Firewall::blocked(std::string& ipToCheck) {
  long long ipConvert = ipToNum(ipToCheck);

  for(vector<long long>& range : ranges) {
    if(ipConvert < range[0] || ipConvert > range[1]) {
      return false;
    }

    return true;
  }
}