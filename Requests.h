/**
 * @file Requests.h
 * @brief Request type and random-number helper for the load balancer simulation.
 */
#ifndef REQUEST_H
#define REQUEST_H

#include <string>

/**
 * @brief Returns a uniformly distributed random integer.
 * @param min Lowest possible value (inclusive).
 * @param max Highest possible value (inclusive).
 * @return A random integer in [min, max].
 */
int randomInt(int min, int max);

/**
 * @brief A single web request moving through the load balancer.
 */
struct Request {
  std::string ipIN;   ///< Source IP address (dotted quad).
  std::string ipOUT;  ///< Destination IP address (dotted quad).

  int time;           ///< Number of clock cycles needed to process the request.
  char jobType;       ///< Job type: 'P' (processing) or 'S' (streaming).

  /**
   * @brief Fills the request with random data.
   *
   * Assigns random source and destination IPs, a random job type ('P' or 'S'),
   * and a processing time chosen from [min, max].
   *
   * @param min Minimum processing time in clock cycles.
   * @param max Maximum processing time in clock cycles.
   */
  void randomize(int min, int max);

  /**
   * @brief Generates a random IPv4 address.
   * @return An address string of the form "a.b.c.d" with each octet in [0, 255].
   */
  std::string randomIP();
};

#endif
