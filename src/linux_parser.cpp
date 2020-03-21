#include <dirent.h>
#include <unistd.h>
#include <set>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::set;
using std::stof;
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
  string line, name;
  int value = 0;
  vector<int> values;
  set<string> memory;
  memory.insert("MemTotal:");
  memory.insert("MemAvailable:");
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (memory.find(name) != memory.end()) {
        values.push_back(value);
      }
      if (values.size() == 2) {
        break;
      }
    }
  }
  return (values[0] - values[1]) * 1.0 / values[0]; 
}

// Read and return the system uptime
long LinuxParser::UpTime() {
  float upTime, idleTime = 0;
  string line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> upTime >> idleTime;
  }
  return (long) upTime; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() {
  vector<string> jiffies = CpuUtilization();
  long total_since_boot = 0;
  for (string s: jiffies) {
    total_since_boot += std::atol(s.c_str());
  }
  return total_since_boot;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() {
  return Jiffies() - IdleJiffies();
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() {
  vector<string> jiffies = CpuUtilization();
  return std::atol(jiffies[3].c_str());
}

// Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() {
  string line, tag;
  vector<string> cpu;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag;
      if (tag == "cpu") {
        while (linestream >> tag) {
        cpu.push_back(tag);
        }
        break;
      }
    }
  }
  return cpu;
}

// Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line, tag;
  int numberOfProcesses = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> numberOfProcesses;
      if (tag == "processes") {
        break;
      }
    }
  }
  return numberOfProcesses;
}

// Read and return the number of running processes
int LinuxParser::RunningProcesses() {
  string line, tag;
  int runningProcesses = 0;
  std::ifstream stream(kProcDirectory + kStatFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> tag >> runningProcesses;
      if (tag == "procs_running") {
        break;
      }
    }
  }
  return runningProcesses;
}

// Read and return the command associated with a process
string LinuxParser::Command(int pid) {
  string cmd = "", line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      cmd += line;
    }
  }
  return cmd;
}

// Read and return the memory used by a process
string LinuxParser::Ram(int pid) {
  string ram = "", line, name, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (name == "VmSize:") {
        ram = std::to_string(std::stoll(value) / 1000);
      }
    }
  }
  return ram;
}

// Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) {
  string uid = "", line, name, value;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::istringstream linestream(line);
      linestream >> name >> value;
      if (name == "Uid:") {
        uid = value;
      }
    }
  }
  return uid;
}

// Read and return the user associated with a process
string LinuxParser::User(int pid) {
  string user = "", x, line;
  int id;
  std::ifstream stream(kPasswordPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      linestream >> user >> x >> id;
      if (id == pid) {
        break;
      }
    }
  }
  return user;
}

// Read and return the uptime of a process
long LinuxParser::UpTime(int pid) {
  string value, line;
  std::ifstream stream(kProcDirectory + std::to_string(pid) + kStatFilename);
  long uptime = 0;
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    for(int i = 1; i < 22; i++) {
      linestream >> value;
    }
    linestream >> uptime;
    uptime /= (_SC_CLK_TCK);
  }
  return uptime;
}