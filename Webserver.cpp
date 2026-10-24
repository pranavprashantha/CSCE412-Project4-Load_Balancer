#include "Webserver.h"

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

  if(timeLeftForCurrent == 0) {
    return false;
  }
  return true;
}

bool Webserver::isFree() {
  return timeLeftForCurrent == 0;
}

void Webserver::getID() {
  return id;
}

void Webserver::startAbort() {
  abort = true;
}

int Webserver::isAborting() {
  return abort;
}