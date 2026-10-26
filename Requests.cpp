#include "Requests.h"

#include <random>

using namespace std;

int randomInt(int min, int max) {
  static std::mt19937 gen(std::random_device{}());
  std::uniform_int_distribution<int> dist(min, max);
  return dist(gen);
}

std::string Request::randomIP() {

  string IP = to_string(randomInt(0, 255)) + "." + to_string(randomInt(0, 255)) + "." + to_string(randomInt(0, 255)) + "." + to_string(randomInt(0, 255));

  return IP;

}

void Requests::randomize(int min, int max) {
  this->ipIN = randomIP();
  this->ipOUT = randomIP();

  this->time = randomIP(min, max);

  int pOrS = randomInt(0,1);

  if(pOrS) {
    this->jobType = 'P';
  } else {
    this->jobType = 'S';
  }
}