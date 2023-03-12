#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>
#include "linux_parser.h"

using std::stof;
using std::stol;
using std::stoi;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
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

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
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

// Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line, key, value;
  float TotalMemory, FreeMemory, AvailableMem;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
        linestream >> key >> value;
        if (key == "MemTotal:") {
          TotalMemory = stof(value);
        } else if (key == "MemFree:") {
          FreeMemory = stof(value);
        }
      }
  }
  AvailableMem = TotalMemory - FreeMemory;
  return AvailableMem/TotalMemory;
 }

long LinuxParser::UpTime() {   
  string line, uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> uptime;
  } else {return -1;}
  return stol(uptime);
  }

long LinuxParser::Jiffies() {return LinuxParser::ActiveJiffies() + LinuxParser::IdleJiffies();}


long LinuxParser::ActiveJiffies(int pid) {   
  string line, value;
  long activetime = 0.0;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 17; ++i) {
      linestream >> value;
      if (i >12) {activetime+=stol(value);
    }
    }
  }
  return activetime;
  }


long LinuxParser::ActiveJiffies() { 
  string line, key, user, nice, system, idle, iowait, irq, softirq, steal;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu") {
        linestream >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
        return stol(user) + stol(nice) + stol(system) + stol(irq) + stol(softirq) + stol(steal);
      }
    }
  }
  return -1.0;
 }

// Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  string line, key, idle, iowait,temp;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> key;
      if (key == "cpu"){
        linestream >> temp >> temp >> temp >> idle >> iowait;
        return stol(idle) + stol(iowait);
      }
      }
    }
    return -1.0;
}


long int LinuxParser::CpuUtilization() { 
  return 1.0*LinuxParser::ActiveJiffies() / LinuxParser::Jiffies();
  }


int LinuxParser::TotalProcesses() { 
  string line, key, value;
  int numprocess;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "processes") {
          numprocess = stoi(value);
          return numprocess;
        }
      }
    }
  }
  return -1;
}

int LinuxParser::RunningProcesses() { 
  string line, key, value;
  int numrunprocess;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "procs_running") {
          numrunprocess= stoi(value);
          return numrunprocess;
        }
      }
    }
  }
  return -1;
 }


string LinuxParser::Command(int pid) { 
  string command;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (filestream.is_open()) {
    std::getline(filestream, command);
  }
  return command;
 }


string LinuxParser::Ram(int pid) { 
  string line, key, val;
  int ramutilkb;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
        linestream >> key >> val;
        if (key == "VmSize:") {
          ramutilkb=stoi(val);
          // convert format value to MB
          return to_string(ramutilkb/1000);
        }
      }
  }
  return val;
 }

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  string line, key, val;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> key >> val) {
        if (key == "Uid:") {
          return val;
        }
      }
    }
  }
  return val;
}


string LinuxParser::User(int pid) { 
  string line, usern, pwd, uid, user;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> usern >> pwd >> uid) {
        if (uid == Uid(pid)) {
          return usern;
        }
      }
    }
  }
  return usern;
 }


long LinuxParser::UpTime(int pid) { 
  string line, val, uptime;
  long procuptime;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatFilename);
  if (filestream.is_open()) {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    for (int i = 0; i < 21; i++) {
      linestream >> val;
    }
      linestream >> uptime;
      procuptime = stol(uptime);
      return UpTime()- procuptime / sysconf(_SC_CLK_TCK);
  }
  return -1;
}