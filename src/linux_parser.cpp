#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iostream>

#include "linux_parser.h"
#include "processor.h"

#define HERTZ sysconf(_SC_CLK_TCK)
#define FILESTREAM_EXCEPTION "The filestream failed to open properly."
#define DATA_EXCEPTION "Expected data not found."

using std::stoi;
using std::stof;
using std::stoll;
using std::string;
using std::to_string;
using std::vector;

string LinuxParser::OperatingSystem() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kOSPath);
    
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
        throw DATA_EXCEPTION;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

string LinuxParser::Kernel() {
    string os, version, kernel;
    string line;
    std::ifstream stream(kProcDirectory + kVersionFilename);
    
    if (stream.is_open()) {
        std::getline(stream, line);
        std::istringstream linestream(line);
        linestream >> os >> version >> kernel;
        return kernel;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

vector<int> LinuxParser::Pids() {
    vector<int> pids;
    DIR* directory = opendir(kProcDirectory.c_str());
    struct dirent* file;
    while ((file = readdir(directory)) != nullptr) {
    if (file->d_type == DT_DIR) {
        string filename(file->d_name);
        if (std::all_of(filename.begin(), filename.end(), isdigit)) {
            int pid = stoi(filename);
            pids.push_back(pid);
            }
        }   
    }
    closedir(directory);
    return pids;
}

float LinuxParser::MemoryUtilization() {
    string category, kilobyte_string, mem_total_string, mem_free_string;
    string line;
    int mem_total_val, mem_free_val;
    float mem_used_kb;
    float mem_utilization;
    std::ifstream stream(kProcDirectory + kMeminfoFilename);
    
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

        mem_used_kb = mem_total_val - mem_free_val;
        mem_utilization = mem_used_kb / mem_total_val;
        return mem_utilization;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

float LinuxParser::CpuUtilization() {
    return 0;
} 

long LinuxParser::UpTime() {
    string line;
    string up_string, idle_string;
    long uptime;
    std::ifstream filestream(kProcDirectory + kUptimeFilename);
    if (filestream.is_open()) {
        if (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> up_string >> idle_string;
        }

        uptime = lround(stod(up_string));
        return uptime;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

long LinuxParser::Jiffies() {
    string line;
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    int jiffies;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);

        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;

        jiffies = stoll(user) + stoll(nice) + stoll(system) + stoll(idle) + stoll(iowait) + stoll(irq) + stoll(softirq) + stoll(steal) + stoll(guest) + stoll(guest_nice);

        return jiffies;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

long LinuxParser::ActiveJiffies() {
    string line;
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    int active_jifs;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);

        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        active_jifs = stoll(user) + stoll(nice) + stoll(system) + stoll(irq) + stoll(softirq) + stoll(steal);

        return active_jifs;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

long LinuxParser::IdleJiffies() {
    string line;
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    long idle_jifs;
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);

        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
        idle_jifs = stoll(idle) + stoll(iowait);

        return idle_jifs;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

int LinuxParser::TotalProcesses() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kProcDirectory + kStatFilename);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "processes") {
                return stoi(value);
            }
        }
        throw DATA_EXCEPTION;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

int LinuxParser::RunningProcesses() {
    string line;
    string key;
    string value;
    std::ifstream filestream(kProcDirectory + kStatFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "procs_running") {
                return stoi(value);
            }
        }
        throw DATA_EXCEPTION;
    } else {
    throw FILESTREAM_EXCEPTION;
    }
}

long LinuxParser::ActiveJiffies(int pid) {
    string line;
    string value;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
    int user_time, kernel_time, cutime, cstime;
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            for (int i = 1; i < 18; i++) {
                linestream >> value;
                if (i == 14) {
                    user_time = stoll(value);
                } else if (i == 15) {
                    kernel_time = stoll(value);
                } else if (i == 16) {
                    cutime = stoll(value);
                } else if (i == 17) {
                    cstime = stoll(value);
                }
            }
            return user_time + kernel_time + cutime + cstime;
        }
        throw DATA_EXCEPTION;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

string LinuxParser::Command(int pid) {
    string line, command;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kCmdlineFilename);

    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> command;
        return command;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

float LinuxParser::CpuUtilization(int pid_) {
    string line, value, starttime;
    float cpu_utilization;
    int seconds;
    int uptime = UpTime();
    int total_time = ActiveJiffies(pid_);

    std::ifstream filestream(kProcDirectory + "/" + to_string(pid_) + kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);

        for (int i = 0; i < 22; i++) {
            linestream >> starttime;
            }

        seconds = uptime - (stoll(starttime) / HERTZ);
        cpu_utilization = (((float) total_time / (float) HERTZ) / (float) seconds);
        return cpu_utilization;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

string LinuxParser::Ram(int pid) {
    string line, key, value, ram_kb{"0"};
    int ram_mb;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            if (linestream >> key >> value) {
                if (key == "VmSize:") {
                    ram_kb = value;
                    ram_mb = stoll(ram_kb) / 1000;
                    return to_string(ram_mb);
                }
            }
        }
        throw DATA_EXCEPTION;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

string LinuxParser::Uid(int pid) {
    string line, key, value;
    string uid;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "Uid:") {
                uid = value;
                return uid;
            }
        }
        throw DATA_EXCEPTION;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

string LinuxParser::User(int pid) {
    string line, key, x, value;
    string user;
    string uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);

    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::replace(line.begin(), line.end(), ':', ' ');
            std::istringstream linestream(line);
            linestream >> key >> x >> value;
            if (value == uid) {
                user = key;
                return user;
            }
        }
        throw DATA_EXCEPTION;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}

long LinuxParser::UpTime(int pid) {
    string line, key;
    string starttime;
    long uptime;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatFilename);

    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for (int i=1; i < 23; i++) {
              linestream >> starttime;
        }
        uptime = (stoll(starttime) / HERTZ);
        return uptime;
    } else {
        throw FILESTREAM_EXCEPTION;
    }
}