#include "processor.h"

// Returns the aggregate CPU utilization based on Vangelis Tasoulas answers
// to this question https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization()
{
  std::vector<std::string> cpuValues = LinuxParser::CpuUtilization();

  // TotalIdle = idle + iowait
  float idle = std::stof(cpuValues[3]) + std::stof(cpuValues[4]);
  // TotalNonIdle = user + nice + system + irq + softirq + steal
  float nonIdle = std::stof(cpuValues[0]) + std::stof(cpuValues[1]) + std::stof(cpuValues[2]) + std::stof(cpuValues[5]) + std::stof(cpuValues[6]) + std::stof(cpuValues[7]);
  float total = idle + nonIdle;

  float cpu_percentage;
  try
  {
    cpu_percentage = ((total - prevTotal) - (idle - prevIdle)) / (total - prevTotal);
  } catch(...) {
    cpu_percentage = 0.0f;
  }

  prevIdle = idle;
  prevNonIdle = nonIdle;
  prevTotal = total;

  return cpu_percentage;
}
