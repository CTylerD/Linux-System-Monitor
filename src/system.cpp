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

int System::RunningProcesses() {
  procs_running = LinuxParser::RunningProcesses();
  return procs_running;
}

int System::TotalProcesses() {
  processes = LinuxParser::TotalProcesses();
  return processes;
}

long System::UpTime() {
  string line;
  string up_string, idle_string;
  string formatted_uptime;
  std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kUptimeFilename);
  if (filestream.is_open()) {
    if (std::getline(filestream, line)) {
    std::istringstream linestream(line);
    linestream >> up_string >> idle_string;
    }
    uptime = lround(stod(up_string));
  }
  return uptime;
}
