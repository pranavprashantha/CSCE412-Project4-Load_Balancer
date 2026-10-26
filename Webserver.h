#ifndef WEBSERVER_H
#define WEBSERVER_H

#include "Requests.h"

class Webserver {
  private:
    static int nextID;
    int id;
    int timeLeftForCurrent;
    bool abort;

    Request r;

  public:

    Webserver();
    void assign(Request& r);
    bool runCycle();

    bool isFree();
    int getID();

    void startAbort();
    bool isAborting();



};

#endif