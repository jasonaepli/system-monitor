#include <string>
#include <chrono>
#include "format.h"

using std::string;

// INPUT: Long int measuring seconds
// OUTPUT: HH:MM:SS

string Format::ElapsedTime(long seconds) { 

  int hours = seconds / 3600;
  
  seconds = seconds % 3600;

  int mins = seconds / 60;

  seconds = seconds % 60;

  std::string hh = std::to_string(hours);
  std::string mm = std::to_string(mins);
  std::string ss = std::to_string(seconds);

  return hh + ":" + mm + ":" + ss; 

}
