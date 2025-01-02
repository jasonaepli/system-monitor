#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <map>
#include "linux_parser.h"
#include <cmath>

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Read the OS name from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// Read the kernel name from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
 return pids;
}

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  
  string mem_total_str, mem_free_str, line, word;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);
  // If stream successfully opened
  if (stream.is_open()) {

	// Extract the number of kB from MemTotal:
	std::getline(stream, line);
	std::istringstream mem_total_stream(line);
  	mem_total_stream >> word >> mem_total_str;

	// Extract the number of kB from MemFree:
	std::getline(stream, line);
	std::istringstream mem_free_stream(line);
	mem_free_stream >> word >> mem_free_str;
 	
  }

  float mem_total = std::stof(mem_total_str);
  float mem_free = std::stof(mem_free_str);
	
  return (mem_total - mem_free) / mem_total; 

}

// Read and return the system uptime
long LinuxParser::UpTime() { 

  string sys_uptime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  // If stream successfully opened
  if (stream.is_open()) {
	// While there are characters still left to read
	std::getline(stream, line);
	std::istringstream linestream(line);
  	// Extract the first word of the line
	linestream >> sys_uptime;
   }
	// Convert the string to an integer
 
  return std::stol(sys_uptime); 

}

// TODO: Read and return the number of jiffies for the system
//long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
//long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
//long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
//long LinuxParser::IdleJiffies() { return 0; }

// Read and return CPU utilization
std::map<std::string, float> LinuxParser::CpuUtilization() {

  string line, word, user_str, nice_str, sys_str, idle_str;
  
  std::ifstream stream(kProcDirectory + kStatFilename);
  
  // If stream successfully opened
  if (stream.is_open()) {
    // While there are characters still left to read
    std::getline(stream, line);
    std::istringstream linestream(line);
  	
    // Extract the cpu utilizaiton values from the line
    linestream >> word >> user_str >> nice_str >> sys_str >> idle_str;
  }
  
  return { {"user", stof(user_str)}, {"nice", stof(nice_str)}, {"system", stof(sys_str)}, {"idle", stof(idle_str)} };
}

//Read and return the total number of processes
int LinuxParser::TotalProcesses() { 

  string procs_str, line, word;
  std::ifstream stream(kProcDirectory + kStatFilename);
  // If stream successfully opened
  if (stream.is_open()) {
	// While there are characters still left to read
	while(std::getline(stream, line)) {
		std::istringstream linestream(line);
  		// Extract the first word of the line
		linestream >> word;
			// If the word is "procs_running"
			if(word == "processes"){
				// Extract the second word and exit the while loop
				linestream >> procs_str;
				break;
			}

  	}
   }
	// Convert the string to an integer
   return std::stoi(procs_str); 
}

//Read and return the number of running processes
int LinuxParser::RunningProcesses() { 

  string procs_str, line, word;
  std::ifstream stream(kProcDirectory + kStatFilename);
  // If stream successfully opened
  if (stream.is_open()) {
	// While there are characters still left to read
	while(std::getline(stream, line)) {
		std::istringstream linestream(line);
  		// Extract the first word of the line
		linestream >> word;
			// If the word is "procs_running"
			if(word == "procs_running"){
				// Extract the second word and exit the while loop
				linestream >> procs_str;
				break;
			}

  	}
   }
	// Convert the string to an integer
   return std::stoi(procs_str); 
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  
  string cmd_str, line, word;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);

  // If stream successfully opened
  if (stream.is_open()) {

    // Read the command line
    std::getline(stream, line);
   }
  
  return line; 

}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  
  string vm_str, line, word, vm_size_mb; 
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  // If stream successfully opened
  if (stream.is_open()) {

    // While there are characters still left to read
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);

      // Extract the first word of the line
      linestream >> word;

      // If the word is "Uid:"
      if(word == "VmSize:"){

        // Extract the next word and exit the while loop
	linestream >> vm_str;
	break;
	}
      }
   }
  if(vm_str.size() > 0) {
    // Convert from kB to MB
    vm_size_mb = std::to_string( static_cast<int>( std::round( std::stof(vm_str) / 1000.0 ) ) );
  }else
   vm_size_mb = "0";
 
  return vm_size_mb;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 

  string uid_str, line, word;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);

  // If stream successfully opened
  if (stream.is_open()) {

    // While there are characters still left to read
    while(std::getline(stream, line)) {
      std::istringstream linestream(line);

      // Extract the first word of the line
      linestream >> word;

      // If the word is "Uid:"
      if(word == "Uid:"){

        // Extract the next word and exit the while loop
	linestream >> uid_str;
	break;
	}
      }
   }
  
  return uid_str; 

}

// Read and return the user associated with a process
string LinuxParser::User(int pid) { 

  std::string uid = LinuxParser::Uid(pid);

  std::ifstream filestream(kPasswordPath);

  std::string token, uname, line;
  char delim = ':';
  int word_count = 0;
 
  std::vector<std::string> token_vec;

  if(filestream.is_open()) {

    while(std::getline(filestream, line)) {
      std::istringstream linestream(line);
      
      while(std::getline(linestream, token, delim)) {
        if(token == uid) {
        
          uname = token_vec[word_count-2];
          break;
      
        }else {
        
          token_vec.push_back(token);
          word_count++;
        }
      }
    }
  }

  return uname; 

 
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) { 

  string ticks_str, line, word;

  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);

  // If stream successfully opened
  if (stream.is_open()) {

    // While there are characters still left to read
    std::getline(stream, line);
    std::istringstream linestream(line);

    for(int i = 0; i < 22; i++)  // Extract the 22nd token from the line in the stat file
      linestream >> ticks_str;

   }
  // Conver the clock ticks to seconds using the conversion in unistd.h
  long uptime = std::stol(ticks_str) / sysconf(_SC_CLK_TCK);
 
  return uptime;

}
