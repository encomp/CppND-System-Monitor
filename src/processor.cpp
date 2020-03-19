#include <string>
#include <vector>
#include "processor.h"

using std::vector;
using std::string;
using std::stoi;

Processor::Processor(vector<string> cpu) {
    if (cpu.size() == 10) {
        user = stoi(cpu[0]);
        nice = stoi(cpu[1]);
        system = stoi(cpu[2]);
        idle = stoi(cpu[3]);
        iowait = stoi(cpu[4]);
        irq = stoi(cpu[5]);
        softirq = stoi(cpu[6]);
        steal = stoi(cpu[7]);
        guest = stoi(cpu[8]);
        guestNice = stoi(cpu[9]);
        init = true;
    } else {
        init = false;
    }
}

// Return the aggregate CPU utilization
float Processor::Utilization() {
    float Cpu = 0.0;
    if (init) {
        long Idle = idle + iowait;
        long NonIdle = user + nice + system + irq + softirq + steal;
        long Total = Idle + NonIdle;
        Cpu = (Total - Idle) * 100 / Total;
        Cpu /= 100;
    }
    return Cpu;
}