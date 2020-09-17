#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>
#include <math.h>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

Processor& System::Cpu() { 
    static Processor cpu;
    return cpu;}

vector<Process>& System::Processes() {
    processes.clear();
    for (int pid : LinuxParser::Pids()) {
        Process process(pid);
        processes.emplace_back(process);
    }
    std::sort(processes.begin(), processes.end());
    return processes;
}

std::string System::Kernel() {
  kernel = LinuxParser::Kernel();
  return kernel;
}

float System::MemoryUtilization() {
  mem_utilization = LinuxParser::MemoryUtilization();
  return mem_utilization;
}

std::string System::OperatingSystem() {
  operating_system_ = LinuxParser::OperatingSystem();
  return operating_system_;
}

int System::RunningProcesses() {
  procs_running = LinuxParser::RunningProcesses();
  return procs_running;
}

int System::TotalProcesses() {
  total_processes = LinuxParser::TotalProcesses();
  return total_processes;
}

long System::UpTime() {
    uptime = LinuxParser::UpTime();
    return uptime;
}
