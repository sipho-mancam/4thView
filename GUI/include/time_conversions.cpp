#include "time_conversions.hpp"
#include <sstream>
#include <iomanip>



std::string Int2TimeString::int2TimeString(int timeInMs)
{
	std::string timeString = "";
	json globalConfig = KEvents::__load_config__();
	double frameRate = globalConfig["systemSettings"]["frameRate"];

	double elapsedSeconds = static_cast<double>(timeInMs) / frameRate;
	int minutes = elapsedSeconds / 60;
	int seconds = static_cast<int>(elapsedSeconds) % 60; 

	std::ostringstream oss;
	oss << std::setfill('0') << std::setw(2) << minutes << ":"
		<< std::setfill('0') << std::setw(2) << seconds;

	timeString = oss.str();
	return timeString;
}
