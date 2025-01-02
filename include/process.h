#ifndef PROCESS_H
#define PROCESS_H
#include <linux_parser.h>
#include <string>
/*
Basic class for Process representation
*/
class Process {
 public:
  Process(int pid) {
    pid_ = pid;
    cmd_ = LinuxParser::Command(pid);
    user_ = LinuxParser::User(pid);
  }
  int Pid();                              
  std::string User();                     
  std::string Command();                  
  float CpuUtilization() const;                 
  std::string Ram();                       
  long int UpTime();                      
  bool operator<(Process const& a) const;
 
  // Declare private members
 private:
  int pid_;		// Process ID
  std::string user_;	// User that started process
  std::string cmd_;	// Command that started the process
};

#endif
