#ifndef PROCESS_H
#define PROCESS_H

#include <string>
#include "linux_parser.h"
#include "processor.h"

class Process {
  public:
    Process(int pid) : pid_(pid) {
    	CpuUtilization();
    }
    int Pid();
    std::string User();
    std::string Command();
    float CpuUtilization();                
    std::string Ram();                     
    long int UpTime();                     
    bool operator<(Process const& a) const;

   private:
     int pid_;
     float cpu_utilization_;
     std::string command_;
     std::string ram_;
     std::string user_;
     long uptime_;
};

#endif