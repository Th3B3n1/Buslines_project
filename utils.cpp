#include "utils.h"

int parse_minutes(const std::string &time_str)
{
	if (time_str.empty())
		return -1;
	size_t separator = time_str.find(':');
	if (separator == std::string::npos)
	{
		return std::stoi(time_str);
	}
	int hours = std::stoi(time_str.substr(0, separator));
	int minutes = std::stoi(time_str.substr(separator + 1));
	return hours * 60 + minutes;
}

std::string format_minutes(int total_minutes)
{
	if (total_minutes < 0)
		return "00:00";
	int hours = total_minutes / 60;
	int minutes = total_minutes % 60;
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes;
	return ss.str();
}