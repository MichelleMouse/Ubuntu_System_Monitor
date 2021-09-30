#include "processor.h"

// Returns the aggregate CPU utilization based on Vangelis Tasoulas answers
// to this question https://stackoverflow.com/questions/23367857/accurate-calculation-of-cpu-usage-given-in-percentage-in-linux
float Processor::Utilization()
{
  long activeJiffies = LinuxParser::ActiveJiffies();
  long idleJiffies = LinuxParser::IdleJiffies();

  float total = idleJiffies + activeJiffies;

  float cpu_percentage = ((total - prevTotal_) - (idleJiffies - prevIdle_)) / (total - prevTotal_);

  prevIdle_ = idleJiffies;
  prevNonIdle_ = activeJiffies;
  prevTotal_ = total;

  return cpu_percentage;
}
