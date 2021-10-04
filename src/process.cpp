#include "process.h"

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
  long activeJiffies = LinuxParser::ActiveJiffies(pid_);
  long procUpTime = LinuxParser::UpTime(pid_);

  long seconds = LinuxParser::UpTime() - procUpTime;

  return activeJiffies / seconds;

}

// Returns the command that generated this process
std::string Process::Command()
{
  return LinuxParser::Command(pid_);
}

// Returns this process's memory utilization
std::string Process::Ram()
{
  ram_ = LinuxParser::Ram(pid_);
  return ram_;
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
  } else if (cpuUse_ == a.CpuUse() && ram_ < a.ram_) {
    return true;
  }
  return false;

}
