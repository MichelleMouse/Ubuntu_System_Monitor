#ifndef PROCESS_H
#define PROCESS_H

#include <cctype>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>

#include

//Basic class for Process representation
class Process {
public:
  Process(int id);
  int Pid();                               // TODO: See src/process.cpp
  std::string User();                      // TODO: See src/process.cpp
  std::string Command();                   // TODO: See src/process.cpp
  float CpuUtilization();                  // TODO: See src/process.cpp
  std::string Ram();                       // TODO: See src/process.cpp
  long int UpTime();                       // TODO: See src/process.cpp
  bool operator<(Process const& a) const;  // TODO: See src/process.cpp

  // TODO: Declare any necessary private members
private:
  int pid_;
  long int upTime_;
  float cpuUse_;
  std::string user_, command_, ram_;
};

#endif
