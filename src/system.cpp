#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"
#include "processor.h"
#include "system.h"
#include <iostream>
using std::set;
using std::size_t;
using std::string;
using std::vector;

// TODO: Return the system's CPU
Processor& System::Cpu() { return cpu_; }

// TODO: Return a container composed of the system's processes
vector<Process>& System::Processes() { 

  // Debug
  //std::cout << "Clearing processes_" << std::endl;

  this->processes_.clear();

  // Debug
  //std::cout << "processes_ is cleared" << std::endl;

  for( int pid : LinuxParser::Pids() ) {

    // Debug
    //std::cout << "Creating Process: " << pid << " and adding to processes_" << std::endl;
    this->processes_.push_back(Process(pid));

    // Debug
    //std::cout << "Process " << pid << " created and added to processes_" << std::endl;

  }

  // Debug
  //std::cout << "******** Returning from Process() *********" << std::endl; 
  return this->processes_;

}

// Return the system's kernel identifier (string)
std::string System::Kernel() { return LinuxParser::Kernel(); }

// Return the system's memory utilization
float System::MemoryUtilization() { return LinuxParser::MemoryUtilization(); }

// Return the operating system name
std::string System::OperatingSystem() { return LinuxParser::OperatingSystem(); }

// Return the number of processes actively running on the system
int System::RunningProcesses() { return LinuxParser::RunningProcesses(); }

// Return the total number of processes on the system
int System::TotalProcesses() { return LinuxParser::TotalProcesses(); }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return LinuxParser::UpTime(); }
