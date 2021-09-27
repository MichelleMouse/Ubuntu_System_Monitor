#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// Returns the pretty name of the operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream stream(kOSPath);
  if (stream.is_open()) {
    while (std::getline(stream, line)) {
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

// Returns the kernel version i.e. 5.xx.x.xx-generic
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

// Returns the PIDs folder name
vector<int> LinuxParser::Pids() {
  vector<int> pids; //Modify this to std::vector<std::string> for full path
  const std::filesystem::path sysFile{kProcDirectory};

  for(std::filesystem::directory_entry const& dir_entry : std::filesystem::directory_iterator{sysFile})
  {
    if(std::filesystem::is_directory(dir_entry))
    {
      std::string dir = dir_entry.path().filename().string(); //Modify this to dir_entry.path().string() to get full path instead
      if(std::all_of(dir.begin(), dir.end(), isdigit))
      {
        pids.push_back(std::stoi(dir));
      }
    }
  }

  return pids;
}

/*
** Reads and returns the system memory utilization
** Calculates usedMemory following htop's creator explanation here
** https://stackoverflow.com/questions/41224738/how-to-calculate-system-memory-usage-from-proc-meminfo-like-htop/41251290#41251290
*/
float LinuxParser::MemoryUtilization()
{
  float totalMem, freeMem;
  std::string key, value, kb, line;
  std::ifstream stream(kProcDirectory + kMeminfoFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb) {
        if (key == "MemTotal") {
          totalMem = std::stof(value);
        } else if (key == "MemFree") {
          freeMem = std::stof(value);
        }
      }
    }
  }

  return ((totalMem - freeMem) / totalMem);
}

// Reads and returns the system uptime
long LinuxParser::UpTime()
{
  std::string sysUpTime, idleTime, line;
  std::ifstream stream(kProcDirectory + kUptimeFilename);

  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> sysUpTime >> idleTime;
  }
  return std::stol(sysUpTime);
}

// Returns the number of jiffies for the system
long LinuxParser::Jiffies()
{
  long upTime = UpTime();
  long jiffies;
  try
  {
    jiffies = upTime/sysconf(_SC_CLK_TCK);
  } catch (...) {
    jiffies = 0;
  }

  return jiffies;
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]])
{
  return 0;
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  return 0;
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  return 0;
}

// Reads and returns CPU utilization
vector<string> LinuxParser::CpuUtilization()
{
  std::string key, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  std::string line;
  std::ifstream stream(kProcDirectory + kStatFilename);

  if(stream.is_open())
  {
    while(std::getline(stream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guest_nice)
      {
        if (key == "cpu")
        {
          return {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
        }
      }
    }
  }
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses()
{
  std::string value, key, line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(!key.compare("processes"))
        {
          return std::stoi(value);
        }
      }
    }
  }
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses()
{
  std::string value, key, line;

  std::ifstream filestream(kProcDirectory + kStatFilename);
  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(!key.compare("procs_running"))
        {
          return std::stoi(value);
        }
      }
    }
  }
}

// Reads and returns the command associated with a process
string LinuxParser::Command(int pid)
{
  std::string cmd, line;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kCmdlineFilename);

  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> cmd)
      {
        return cmd;
      }
    }
  }
}

// Reads and returns the memory used by a process
string LinuxParser::Ram(int pid)
{
  float memory, freeMem;
  std::string key, value, kb, line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);

  if (stream.is_open()) {
    while (std::getline(stream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "VmSize") {
          try
          {
            memory = std::stof(value)/1024;
          } catch (...) {
            memory = 0.0;
          }
        }
      }
    }
  }

  return std::string(memory);
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { return string(); }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { return string(); }

// Reads and returns the uptime of a process
// Visit https://man7.org/linux/man-pages/man5/proc.5.html for data explanation
long LinuxParser::UpTime(int pid)
{
  long upTime;
  std::string data, line;
  std::vector<std::string> pidData;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatFilename);

  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> data)
      {
        pidData.push_back(data);
      }
    }
  }

  try
  {
    upTime = pidData[21]/sysconf(_SC_CLK_TCK); //pricess start time
  } catch (...) {
    upTime = 0l;
  }

  return upTime;
}
