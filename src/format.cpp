#include "format.h"

using std::string;

// Formats the running time to HH:MM:SS from seconds input
string Format::ElapsedTime(long sec)
{
  std::chrono::seconds seconds{sec};
  std::chrono::hours hours = std::chrono::duration_cast<std::chrono::hours>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(hours);

  std::chrono::minutes minutes = std::chrono::duration_cast<std::chrono::minutes>(seconds);

  seconds -= std::chrono::duration_cast<std::chrono::seconds>(minutes);

  std::stringstream readableTime{};

  readableTime << std::setw(2) << std::setfill('0') << hours.count()
               << std::setw(1) << ":"
               << std::setw(2) << std::setfill('0') << minutes.count()
               << std::setw(1) << ":"
               << std::setw(2) << std::setfill('0') << seconds.count();

  return readableTime.str();
}
