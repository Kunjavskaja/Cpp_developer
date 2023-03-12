#include <string>
#include <iomanip>
#include "format.h"

using std::string;


string Format::ElapsedTime(long seconds) {
  int secInh= 3600;
  long Hour=seconds/secInh;
  long Minutes=(seconds%secInh)/60;
  long Seconds=(seconds%secInh)%60;
  std::ostringstream elapsedTime;
  elapsedTime << std::setw(2) << std::setfill('0') << Hour
              << ":" << std::setw(2) << std::setfill('0') << Minutes
              << ":" << std::setw(2) << std::setfill('0') << Seconds;
          
  string time = elapsedTime.str();
  return time;
 }