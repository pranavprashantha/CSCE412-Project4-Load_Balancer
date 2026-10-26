#ifndef REQUEST_H
#define REQUEST_H

#include <string>

int randomInt(int min, int max);

struct Request {
  std::string ipIN;
  std::string ipOUT;

  int time;
  char jobType;

  void randomize(int min, int max);
  std::string randomIP(); 
}

#endif