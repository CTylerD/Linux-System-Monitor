#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

int Process::Pid() {
    return pid_;
}

float Process::CpuUtilization() {
    cpu_utilization_ = LinuxParser::CpuUtilization(pid_);
    return cpu_utilization_;
}

string Process::Command() {
    command_ = LinuxParser::Command(pid_);
    return command_;
}

string Process::Ram() {
    ram_ = LinuxParser::Ram(pid_);
    return ram_;
}

string Process::User() {
    user_ = LinuxParser::User(pid_);
    return user_;
}

long int Process::UpTime() {
    int hertz = sysconf(_SC_CLK_TCK);
    uptime_ = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / hertz);
    return uptime_;
}

bool Process::operator<(Process const& a) const {
    return a.cpu_utilization_ < cpu_utilization_;
}