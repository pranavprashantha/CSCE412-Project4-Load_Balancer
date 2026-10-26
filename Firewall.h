/**
 * @file Firewall.h
 * @brief IP-range firewall used to block incoming requests.
 */
#ifndef FIREWALL_H
#define FIREWALL_H

#include <vector>
#include <string>
#include <sstream>

/**
 * @brief Converts a dotted-quad IPv4 address to a single number.
 * @param ip Address in the form "a.b.c.d".
 * @return The address as a 32-bit value stored in a long long, so addresses
 *         can be compared numerically.
 */
long long ipToNum(const std::string& ip);

/**
 * @brief Blocks requests whose source IP falls inside any configured range.
 */
class Firewall {
  private:
    std::vector<std::vector<long long>> ranges; ///< Blocked ranges, each stored as {lower, upper} (inclusive).

  public:
    /**
     * @brief Checks whether an IP address is blocked.
     * @param ipToCheck Address in the form "a.b.c.d".
     * @return True if the address lies inside any blocked range.
     */
    bool blocked(const std::string& ipToCheck);

    /**
     * @brief Adds an inclusive range of IP addresses to block.
     * @param lowerIP First address of the range.
     * @param upperIP Last address of the range.
     */
    void addToBlocked(const std::string& lowerIP, const std::string& upperIP);
};

#endif
