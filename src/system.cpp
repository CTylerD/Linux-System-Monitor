#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <exception>
#include <math.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"
#include "format.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

std::string System::Kernel() {
  kernel = LinuxParser::Kernel();
  return kernel;
}

float System::MemoryUtilization() {
  mem_utilization = LinuxParser::MemoryUtilization();
  return mem_utilization;
}

std::string System::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(LinuxParser::kOSPath);
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
}

int System::RunningProcesses() {
  procs_running = LinuxParser::RunningProcesses();
  return procs_running;
}

int System::TotalProcesses() {
  processes = LinuxParser::TotalProcesses();
  return processes;
}

long System::UpTime() {
    uptime = LinuxParser::UpTime();
    return uptime;
}
