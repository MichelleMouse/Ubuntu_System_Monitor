#include <unistd.h>
#include <cstddef>
#include <set>
#include <string>
#include <vector>

#include "process.h"
#include "processor.h"
#include "system.h"
#include "linux_parser.h"

using std::set;
using std::size_t;
using std::string;
using std::vector;

// Returns the system's CPU
Processor& System::Cpu()
{
  return cpu_;
}

// Returns a container composed of the system's processes
vector<Process>& System::Processes()
{
  std::vector<int> pids = LinuxParser::Pids();
  processes_.clear();

  for(int id : pids)
  {
    Process process(id);
    processes_.push_back(process);
  }

  return processes_;
}

// Returns the system's kernel identifier i.e. 5.xx.x.xx-generic
std::string System::Kernel()
{
  return LinuxParser::Kernel();
}

// Returns the system's memory utilization
// Calculates usedMemory following htop's creator explanation here https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
float System::MemoryUtilization()
{
  return LinuxParser::MemoryUtilization();
}

// Returns the operating system pretty name
std::string System::OperatingSystem()
{
  return LinuxParser::OperatingSystem();
}

// Returns the number of processes actively running on the system
int System::RunningProcesses()
{
  return LinuxParser::RunningProcesses();
}

// Returns the total number of processes on the system
int System::TotalProcesses()
{
  return LinuxParser::TotalProcesses();
}

// Returns the number of seconds since the system started running
long int System::UpTime()
{
  return LinuxParser::UpTime();
}
