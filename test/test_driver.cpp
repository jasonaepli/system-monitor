#include <string>
#include <iostream>
#include "../include/linux_parser.h"
//#include "../src/linux_parser.cpp"

int main() {
  
  int run_proc = LinuxParser::RunningProcesses();

  std::cout << "The number of running processes is: " << run_proc << "\n";

}
