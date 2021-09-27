#include "process.h"

using std::string;
using std::to_string;
using std::vector;

Process::Process(int id)
{
  pid_ = id;
  upTime_ = LinuxParser::UpTime(id);
  cpuUse_;
  user_;
  command_ = LinuxParser::Command(id);
  ram_ = LinuxParser::Ram(id);
}

// Returns this process's ID
int Process::Pid()
{
  return pid_;
}

// Returns the process's CPU utilization
float Process::CpuUtilization()
{
  return cpuUse_;
}

// Returns the command that generated this process
std::string Process::Command()
{
  return command_;
}

// Returns this process's memory utilization
std::string Process::Ram()
{
  return ram_;
}

// Returns the user (name) that generated this process
std::string Process::User()
{
  return user_;
}

// Returns the age of this process (in seconds)
long int Process::UpTime()
{
  return upTime_;
}

// TODO: Overload the "less than" comparison operator for Process objects
// REMOVE: [[maybe_unused]] once you define the function
bool Process::operator<(Process const& a[[maybe_unused]]) const { return true; }
