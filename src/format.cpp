#include "format.h"

using std::string;

// Formats the running time to HH:MM:SS from seconds input
string Format::ElapsedTime(long sec)
{
  int hours, minutes, seconds;
  std::string readableTime;

  hours = int(((sec/60)/60)%60);
  minutes = int((sec/60)%60);
  seconds = int(sec%60);

  if(hours < 10)
  {
    readableTime = "0" + std::to_string(hours);
  } else {
    readableTime = std::to_string(hours);
  }

  if(minutes < 10)
  {
    readableTime += ":0" + std::to_string(minutes);
  } else {
    readableTime += ":" + std::to_string(minutes);
  }

  if(seconds < 10)
  {
    readableTime += ":0" + std::to_string(seconds);
  } else {
    readableTime += ":" + std::to_string(seconds);
  }

  return readableTime;
}
