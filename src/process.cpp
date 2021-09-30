#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(const int id)
{
  pid_ = id;
  CpuUtilization();
  Ram();
}

// Returns this process's ID
int Process::Pid()
{
  return pid_;
}

float Process::CpuUse() const
{
  return cpuUse_;
}

// Returns the process's CPU utilization
float Process::CpuUtilization()
{
  try
  {
    cpuUse_ = (LinuxParser::ActiveJiffies() / (LinuxParser::Jiffies() - LinuxParser::UpTime(pid_))) * 1.0;
  } catch (...) {
    cpuUse_ = 0.0;
  }

  return cpuUse_;
}

// Returns the command that generated this process
std::string Process::Command()
{
  return LinuxParser::Command(pid_);
}

// Returns this process's memory utilization
std::string Process::Ram() const
{
  return LinuxParser::Ram(pid_);
}

// Returns the user (name) that generated this process
std::string Process::User()
{
  return LinuxParser::User(pid_);
}

// Returns the age of this process (in seconds)
long int Process::UpTime()
{
  return LinuxParser::UpTime(pid_);
}

// Overloads the "less than" comparison operator for Process objects
bool Process::operator<(Process const& a) const
{
  if(cpuUse_ > a.CpuUse())
  {
    return true;
  } else {
    return false;
  }
}
