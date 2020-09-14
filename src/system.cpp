#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { return cpu_; }

vector<Process>& System::Processes() { return processes_; }

string System::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// TODO: Return the system's memory utilization
float System::MemoryUtilization() {
  string category, kilobyte_string, mem_total_string, mem_free_string;
  string line;
  std::ifstream stream(LinuxParser::kProcDirectory + LinuxParser::kMeminfoFilename);
  if (stream.is_open()) {
    for (int i = 0; i < 2; i++) {
      std::getline(stream, line);
      std::istringstream linestream(line);
      if (i == 0) {
        linestream >> category >> mem_total_string >> kilobyte_string;
        mem_total_val = std::stoi(mem_total_string);
      } else {
        linestream >> category >> mem_free_string >> kilobyte_string;
        mem_free_val = std::stoi(mem_free_string);
      }
    }
  }
  mem_used_kb = mem_total_val - mem_free_val;
  mem_utilization = mem_used_kb / mem_total_val;
  return mem_utilization;
}

// TODO: Return the operating system name
//std::string System::OperatingSystem() { return string(); }

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
  return value;
}

// TODO: Return the number of processes actively running on the system
int System::RunningProcesses() { return 0; }

// TODO: Return the total number of processes on the system
int System::TotalProcesses() { return 0; }

// TODO: Return the number of seconds since the system started running
long int System::UpTime() { return 0; }