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

// TODO: Return this process's CPU utilization
float Process::CpuUtilization() {
    cpu_utilization_ = LinuxParser::CpuUtilization(pid_);
    return cpu_utilization_;
}

// TODO: Return the command that generated this process
string Process::Command() {
    command_ = LinuxParser::Command(pid_);
    return command_;
}

// TODO: Return this process's memory utilization
string Process::Ram() {
    ram_ = LinuxParser::Ram(pid_);
    return ram_;
}

// TODO: Return the user (name) that generated this process
string Process::User() {
    user_ = LinuxParser::User(pid_);
    return user_;
}

// TODO: Return the age of this process (in seconds)
long int Process::UpTime() {
    int hertz = sysconf(_SC_CLK_TCK);
    uptime_ = LinuxParser::UpTime() - (LinuxParser::UpTime(pid_) / hertz);
    return uptime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a) const {
    return a.cpu_utilization_ < cpu_utilization_;
}