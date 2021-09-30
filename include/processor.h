#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#include <string>
#include <vector>

class Processor {
public:
  float Utilization();

private:
  float prevIdle_ = 0;
  float prevNonIdle_ = 0;
  float prevTotal_ = 0;
};

#endif
