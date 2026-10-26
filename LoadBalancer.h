/**
 * @file LoadBalancer.h
 * @brief Load balancer that queues requests, assigns them to servers, and scales the server pool.
 */
#ifndef LOADBALANCER_H
#define LOADBALANCER_H

#include "Requests.h"
#include "Webserver.h"
#include "Firewall.h"

#include <vector>
#include <ostream>
#include <queue>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

/**
 * @brief Distributes queued requests across a pool of web servers.
 *
 * Each call to cycle() advances the simulation by one clock cycle. Requests
 * from blocked IP ranges are rejected by the firewall. Periodically the pool
 * grows if the queue is too long and shrinks if it is short. Events are
 * written to the terminal (in color) and to a log stream, and start and end
 * summaries report the run's statistics.
 */
class LoadBalancer {
  private:

    int clock;         ///< Current clock cycle.
    int totalCycles;   ///< Cycle at which the end summary is printed.
    bool verbose;      ///< Whether event lines are logged.
    int scaleCycle;    ///< Cycle of the last scaling action.
    int scaleN;        ///< Minimum cycles to wait between scaling actions.

    std::queue<Request> requestQueue; ///< Requests waiting for a server.
    std::vector<Webserver> servers;   ///< Current pool of servers.
    std::ostream& log;                ///< Stream that receives the plain-text log.
    Firewall firewall;                ///< Blocks requests from configured IP ranges.

    // Log statistics

    int requestsPassed = 0;   ///< Requests accepted into the queue.
    int requestsBlocked = 0;  ///< Requests rejected by the firewall.
    int requestsFinished = 0; ///< Requests fully processed by a server.
    int pRequests = 0;        ///< Accepted requests of type 'P'.
    int sRequests = 0;        ///< Accepted requests of type 'S'.

    int serverStartCount = 0; ///< Number of servers at the start.
    int maxServers = 0;       ///< Highest number of active servers reached.
    int serversAdded = 0;     ///< Servers added by scaling.
    int serversRemoved = 0;   ///< Servers removed by scaling.

    int queueStartCount = 0;  ///< Queue size at cycle 0.
    int queueMaxCount = 0;    ///< Largest queue size seen when adding a request.

    /**
     * @brief Counts servers that are not marked for removal.
     * @return The number of active servers.
     */
    int getServerCount();

    /**
     * @brief Logs a line tagged with the current cycle.
     *
     * Does nothing unless verbose is on. The terminal copy is colored; the
     * log stream copy is plain text.
     *
     * @param color ANSI color code used for the terminal output.
     * @param message Text of the event.
     */
    void logEvent(const std::string& color, const std::string& message);


  public:
    /**
     * @brief Creates a load balancer with an initial pool of servers.
     * @param numServers Number of servers to start with.
     * @param scaleCycleNum Minimum cycles to wait between adding or removing servers.
     * @param verbose Whether to log individual events.
     * @param log Stream that receives the plain-text log.
     * @param totalCycles Cycle at which the end summary is printed.
     */
    LoadBalancer(int numServers, int scaleCycleNum, bool verbose, std::ostream& log, int totalCycles);

    /**
     * @brief Submits a request to the load balancer.
     *
     * The request is queued unless the firewall blocks its source IP.
     *
     * @param r The request to submit.
     * @return True if the request was queued, false if it was blocked.
     */
    bool addRequest(Request& r);

    /**
     * @brief Blocks an inclusive range of source IP addresses.
     * @param min First address of the range.
     * @param max Last address of the range.
     */
    void addFirewall(std::string min, std::string max);

    /**
     * @brief Advances the simulation by one clock cycle.
     *
     * Assigns queued requests to free servers, runs every server, removes
     * aborting servers that have finished, then adds or marks a server for
     * removal if the queue size calls for it and the wait time has passed.
     * Prints the end summary when the final cycle is reached.
     */
    void cycle();

    /**
     * @brief Prints the start summary to the terminal and the log.
     */
    void summaryStart();

    /**
     * @brief Prints the end summary (totals and statistics) to the terminal and the log.
     */
    void endSummary();
};

#endif
