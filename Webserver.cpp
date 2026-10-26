#include "Webserver.h"

int Webserver::nextID = 1;

Webserver::Webserver () : id(nextID++), timeLeftForCurrent(0), abort(false), r{} {}

void Webserver::assign(Request& req) {
  r = req;
  timeLeftForCurrent = req.time;
}

bool Webserver::runCycle() {
  if(timeLeftForCurrent == 0) {
    return false;
  }
  timeLeftForCurrent--;

  // true only on the cycle the current request finishes
  return timeLeftForCurrent == 0;
}

bool Webserver::isFree() {
  return timeLeftForCurrent == 0;
}

int Webserver::getID() {
  return id;
}

void Webserver::startAbort() {
  abort = true;
}

bool Webserver::isAborting() {
  return abort;
}