#ifndef SYSTEM_H
#define SYSTEM_H

#include <string>
#include <vector>

#include "process.h"
#include "processor.h"

class System {
 public:
  Processor& Cpu();
  std::vector<Process>& Processes();
  float MemoryUtilization();
  long UpTime();
  int TotalProcesses();
  int RunningProcesses();
  std::string Kernel();
  std::string OperatingSystem();

  std::vector<Process> processes = {};

 private:
  std::string kernel;
  Processor cpu = {};
  int mem_total_val, mem_free_val;
  float mem_used_kb;
  float mem_utilization;
  int total_processes;
  int procs_running;
  long uptime;
  std::string operating_system_;
};

#endif