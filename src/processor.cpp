#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <sstream>
#include <string>
#include <math.h>

using std::string;
using std::stof;

float Processor::Utilization() { 
    string line;
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    float passive, active, total, usage_percentage;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    }

    passive = stof(idle) + stof(iowait);
    active = stof(user) + stof(nice) + stof(system) + stof(irq) + stof(softirq) + stof(steal);
    total = passive + active;
    usage_percentage = (total - passive) / total;

    return roundf(usage_percentage * 100) / 100; 
}
