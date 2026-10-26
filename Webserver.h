/**
 * @file Webserver.h
 * @brief A simulated web server that processes one request at a time.
 */
#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Requests.h"

/**
 * @brief A web server that works on a single request for a number of clock cycles.
 *
 * Each server gets a unique ID when constructed. A server can be marked for
 * removal with startAbort(); it keeps working until its current request is
 * done and is then removed by the LoadBalancer.
 */
class Webserver {
  private:
    static int nextID;      ///< ID given to the next server created.
    int id;                 ///< Unique ID of this server.
    int timeLeftForCurrent; ///< Cycles left on the current request (0 when idle).
    bool abort;             ///< True once the server has been marked for removal.

    Request r;              ///< The request currently being processed.

  public:

    /**
     * @brief Creates an idle server with the next available ID.
     */
    Webserver();

    /**
     * @brief Gives this server a request to process.
     *
     * The server's remaining time is set to the request's processing time.
     *
     * @param r The request to process.
     */
    void assign(Request& r);

    /**
     * @brief Advances the server by one clock cycle.
     * @return True only on the cycle the current request finishes; false if
     *         the server is idle or still working.
     */
    bool runCycle();

    /**
     * @brief Checks whether the server has no request to work on.
     * @return True if the server is idle.
     */
    bool isFree();

    /**
     * @brief Gets the server's unique ID.
     * @return The server ID.
     */
    int getID();

    /**
     * @brief Marks the server for removal once its current request finishes.
     */
    void startAbort();

    /**
     * @brief Checks whether the server has been marked for removal.
     * @return True if startAbort() has been called.
     */
    bool isAborting();

};

#endif
