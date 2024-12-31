#include "processor.h"
#include "linux_parser.h"
#include <map>
#include <string>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 

  std::map<std::string, float> util_map = LinuxParser::CpuUtilization();

  float utilization = util_map["user"] + util_map["nice"] + util_map["system"];

  return utilization / (utilization + util_map["idle"]);
}
