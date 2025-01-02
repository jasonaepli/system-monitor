#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>
#include "linux_parser.h"
#include "process.h"

using std::string;
using std::to_string;
using std::vector;

// Return this process's ID
int Process::Pid() { return this->pid_; }

// Return this process's CPU utilization
float Process::CpuUtilization() const {  

  float util = 0.0f;
  float uptime = static_cast<float>( LinuxParser::UpTime() );
  auto hertz = sysconf(_SC_CLK_TCK);

  string line, token, utime, stime, cutime, cstime, starttime;
  
  std::ifstream stream(LinuxParser::kProcDirectory + std::to_string(this->pid_) + LinuxParser::kStatFilename);

  // If stream successfully opened
  if (stream.is_open()) {
    
    // Get the single line
    std::getline(stream, line);
    std::istringstream linestream(line);

    // Repeated extract a toke over the first 22 tokens
    for(int i = 0; i < 22; i++) {

      linestream >> token;

      switch(i) {
        case 14:
          utime = token;
          break;
        case 15:
          stime = token;
          break;
        case 16:
          cutime = token;
          break;       
        case 17:
          cstime = token;
          break;
        case 22:
          starttime = token;
          break;     
      }

    }

  }

  if(utime.size() != 0 and stime.size() != 0 and cutime.size() != 0 and cstime.size() != 0 and starttime.size() != 0) {
  // Determine the total time spend for the process not including child processes
  float total_time = stof(utime) + stof(stime);
  
  // Determine total elapsed time in seconds since the process started
  float seconds = uptime - ( stof(starttime) / hertz );

  // Determine the CPU usage percentage
  util = 100 * ( (total_time / hertz) / seconds );

  }
  return util; 

}

// Return the command that generated this process
string Process::Command() { return this->cmd_; }

// Return this process's memory utilization
string Process::Ram() { return LinuxParser::Ram(this->pid_); }

// Return the user (name) that generated this process
string Process::User() { return this->user_; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return LinuxParser::UpTime(this->pid_); }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const { 

  return ( a.CpuUtilization() < this->CpuUtilization() ); 

}
