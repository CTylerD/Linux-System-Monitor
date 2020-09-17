 #include "processor.h"
#include "linux_parser.h"

#include <iostream>

float Processor::Utilization() { 
    long total, prev_total, total_delta, idle_delta;
    float usage_percentage;
    
    active_cpu = LinuxParser::ActiveJiffies();
    idle_cpu = LinuxParser::IdleJiffies();

    prev_total = prev_idle_cpu + prev_active_cpu;
    total = active_cpu + idle_cpu;
    
    total_delta = total - prev_total;
    idle_delta = idle_cpu - prev_idle_cpu;
    
    usage_percentage = ((float) total_delta - (float) idle_delta) / (float) total_delta;

    prev_idle_cpu = idle_cpu;
    prev_active_cpu = active_cpu;

    return usage_percentage;
}
