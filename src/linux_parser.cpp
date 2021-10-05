#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

//Genric function to find value in a system file
template <typename T>
T findValueByKey(std::string const &keyFilter, std::string const &fileName)
{
  std::string line, key;
  T value;
  std::ifstream filestream(LinuxParser::kProcDirectory + fileName);

  if(filestream.is_open())
  {
    while(std::getline(filestream, line))
    {
      std::istringstream linestream(line);
      while(linestream >> key >> value)
      {
        if(key == keyFilter)
        {
          return value;
        }
      }
    }
  }
  return value;
};

// Returns the pretty name of the operating system
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream stream(kOSPath);
  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == "PRETTY_NAME")
        {
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
  if (stream.is_open())
  {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// Returns the PIDs folder name
vector<int> LinuxParser::Pids()
{
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
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

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value >> kb)
      {
        if (key == filterTotalMemory)
        {
          totalMem = std::stof(value);
        } else if (key == filterFreeMemory) {
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

  if (stream.is_open())
  {
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

// Reads and returns the number of active jiffies for a PID
long LinuxParser::ActiveJiffies(int pid)
{
  long acttiveJiffies;
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
  //utime + stime + cutime + cstime
  acttiveJiffies = (std::stol(pidData[13]) + std::stol(pidData[14]) + std::stol(pidData[15]) + std::stol(pidData[16]))/sysconf(_SC_CLK_TCK);

  return acttiveJiffies;
}

// Reads and returns the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{
  std::vector<std::string> cpuValues = CpuUtilization();

  return (std::stof(cpuValues[0]) + std::stof(cpuValues[1]) + std::stof(cpuValues[2]) + std::stof(cpuValues[5]) + std::stof(cpuValues[6]) + std::stof(cpuValues[7]))/sysconf(_SC_CLK_TCK);
}

// Reads and returns the number of idle jiffies for the system
long LinuxParser::IdleJiffies()
{
  std::vector<std::string> cpuValues = CpuUtilization();

  return (std::stof(cpuValues[3]) + std::stof(cpuValues[4]))/sysconf(_SC_CLK_TCK);
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
        if (key == filterCPU)
        {
          return {user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice};
        }
      }
    }
  }
  return {};
}

// Reads and returns the total number of processes
int LinuxParser::TotalProcesses()
{
  // std::string value, key, line;
  int totalProcesses = findValueByKey<int>(filterProcesses, kStatFilename);

  return totalProcesses;
}

// Reads and returns the number of running processes
int LinuxParser::RunningProcesses()
{
  int runningProcesses = findValueByKey<int>(filterProcRunning, kStatFilename);

  return runningProcesses;
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
  return string();
}

// Reads and returns the memory used by a process
string LinuxParser::Ram(int pid)
{
  int memoryUsage;
  std::string key, value, line;
  std::ifstream stream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);

  if (stream.is_open())
  {
    while (std::getline(stream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value)
      {
        if (key == filterProcMemory) //Physical RAM size instead of virtual memory (VmSize)
        {
          try
          {
            memoryUsage = std::stoi(value)/1024;
          } catch (...) {
            memoryUsage = 0;
          }
        }
      }
    }
  }

  return std::to_string(memoryUsage);
}

// Reads and returns the user ID associated with a process
std::string LinuxParser::Uid(int pid)
{
  std::string key, data, line, uid;
  std::ifstream filestream(kProcDirectory + "/" + std::to_string(pid) + kStatusFilename);

  if(filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> key >> data)
      {
        if (key == filterUserID)
        {
          try
          {
            return data;
          } catch (...) {
            return string();
          }
        }
      }
    }
  }
  return string();
}

// Reads and returns the user associated with a process
string LinuxParser::User(int pid)
{
  std::string uid = Uid(pid);
  std::string value, username, x, line;
  std::ifstream filestream(kPasswordPath);

  if(filestream.is_open())
  {
    while (std::getline(filestream, line))
    {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while(linestream >> username >> x >> value)
      {
        if(value == uid)
        {
          return username;
        }
      }
    }
  }
  return string();
}

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
    upTime = std::stol(pidData[21])/sysconf(_SC_CLK_TCK); //process start time
  } catch (...) {
    upTime = 0;
  }

  return upTime;
}
