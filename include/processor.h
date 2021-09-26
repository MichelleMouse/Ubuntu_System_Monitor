#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"

#include <string>
#include <vector>

class Processor {
public:
  float Utilization();

private:
  float prevIdle = 0;
  float prevNonIdle = 0;
  float prevTotal = 0;
};

#endif
