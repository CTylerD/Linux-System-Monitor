#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <iostream>

#include "linux_parser.h"

using std::stof;
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
  }
  return value;
}

string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
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
  }

  mem_used_kb = mem_total_val - mem_free_val;
  mem_utilization = mem_used_kb / mem_total_val;
  return mem_utilization;
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
  }
  return uptime;
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
    }
    jiffies = stoi(cpu) + stoi(user) + stoi(nice) + stoi(system) + stoi(idle) + stoi(iowait) + stoi(irq) + stoi(softirq) + stoi(steal) + stoi(guest) + stoi(guest_nice);
    return jiffies;
}

long LinuxParser::ActiveJiffies(int pid) {
  string line;
  string key, value;
  std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatFilename);
  int user_time, kernel_time;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key >> value;
      if (key == "utime") {
        user_time = stoi(value);
      } else if (key == "stime") {
        kernel_time = stoi(value);
      }
    }
  }
  return user_time + kernel_time;
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
    }
    active_jifs =  stoi(cpu) + stoi(user) + stoi(nice) + stoi(system);
    return active_jifs;
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
    }
    idle_jifs = stoi(idle) + stoi(iowait);
    return idle_jifs;
}

float LinuxParser::CpuUtilization() {
    string line;
    string cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
    long cpu_total, cpu_idle;
    float cpu_utilization;
    int user_time{};
    int kernel_time{};
    std::ifstream filestream(LinuxParser::kProcDirectory + LinuxParser::kStatFilename);
    
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice;
    }

    cpu_total = Jiffies();
    cpu_idle = IdleJiffies();
    cpu_utilization= (100 * cpu_total / cpu_idle);

    return cpu_utilization;
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
  }
}

string LinuxParser::Command(int pid) {
    string line, key, value;
    string command;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "Name:") {
            command = value;
        }
    }
    return command;
}

string LinuxParser::Ram(int pid) {
    string line, key, value;
    string ram;
    float ram_mb;
    std::ifstream filestream(kProcDirectory + "/" + to_string(pid) + kStatusFilename);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> value;
            if (key == "VMSize:") {
                ram = value;
                break;
            }
        }
    }
    ram_mb = stoi(ram) / 1000;
    return to_string(ram_mb);
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
                break;
            }
        }
    }
    return uid;
}

string LinuxParser::User(int pid) {
    string line, key, x, value;
    string user;
    string uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
        while (std::getline(filestream, line)) {
            std::istringstream linestream(line);
            linestream >> key >> x >> value;
            if (value == uid) {
                user = key;
                break;
            }
        }
    }
    return user;
}

long LinuxParser::UpTime(int pid) {
    string line, key, value;
    long uptime;
    long system_uptime = LinuxParser::UpTime();
    string uid = Uid(pid);
    std::ifstream filestream(kPasswordPath);
    if (filestream.is_open()) {
        std::getline(filestream, line);
        std::istringstream linestream(line);
        for (int i=0; i < 22; i++) {
            linestream >> value;
        }
        if (stol(value) > 0) {
            uptime = system_uptime - (stol(value)/sysconf(_SC_CLK_TCK));
        }
    }
    return uptime;
}