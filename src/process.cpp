#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int pid, string user, string command, float cpuUtilization, string ram, int upTime) {
    this->pid = pid;
    this->user = user;
    this->command = command;
    this->cpuUtilization = cpuUtilization;
    this->ram = ram;
    this->upTime = upTime;
}

// Return this process's ID
int Process::Pid() { return pid; }

// Return this process's CPU utilization
float Process::CpuUtilization() { return cpuUtilization; }

// Return the command that generated this process
string Process::Command() { return command; }

// Return this process's memory utilization
string Process::Ram() { return ram; }

// Return the user (name) that generated this process
string Process::User() { return user; }

// Return the age of this process (in seconds)
long int Process::UpTime() { return upTime; }

// Overload the "less than" comparison operator for Process objects
bool Process::operator<(Process const &a) const {
    return upTime < a.upTime;
}