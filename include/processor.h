#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <string>
#include <vector>

using std::vector;
using std::string;

class Processor {
 public:
  Processor(vector<string> cpu);
  float Utilization();  // TODO: See src/processor.cpp

  // Declare any necessary private members
 private:
 bool init;
 int user;
 int nice;
 int system;
 int idle;
 int iowait;
 int irq;
 int softirq;
 int steal;
 int guest;
 int guestNice;
};

#endif