#ifndef PROCESS_H
#define PROCESS_H

#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include "linux_parser.h"

//Basic class for Process representation
class Process {
public:
  Process(const int id);
  int Pid();
  std::string User();
  std::string Command();
  float CpuUtilization();
  std::string Ram();
  long int UpTime();
  float CpuUse() const;
  bool operator<(Process const& a) const;

  // TODO: Declare any necessary private members
private:
  int pid_;
  float cpuUse_;
  std::string ram_;
};

#endif
