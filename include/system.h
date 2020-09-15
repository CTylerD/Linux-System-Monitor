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

 private:
  std::string kernel;
  Processor cpu_ = {};
  std::vector<Process> processes_ = {};
  int mem_total_val, mem_free_val;
  float mem_used_kb;
  float mem_utilization;
  int processes;
  int procs_running;
  long uptime;
};

#endif