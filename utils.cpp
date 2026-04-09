#include "utils.h"

int utils::parse_minutes(const std::string &time_str)
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

std::string utils::format_minutes(int total_minutes)
{
	if (total_minutes < 0)
		return "00:00";
	int hours = total_minutes / 60;
	int minutes = total_minutes % 60;
	std::ostringstream ss;
	ss << std::setfill('0') << std::setw(2) << hours << ":" << std::setfill('0') << std::setw(2) << minutes;
	return ss.str();
}

std::tm utils::to_local_tm(const std::chrono::system_clock::time_point &tp)
{
	std::time_t t = std::chrono::system_clock::to_time_t(tp);
	std::tm local_tm = {};
#ifdef _WIN32
	localtime_s(&local_tm, &t);
#else
	localtime_r(&t, &local_tm);
#endif
	return local_tm;
}

std::string utils::format_date(const std::chrono::system_clock::time_point &tp, const char *pattern)
{
    std::tm local_tm = to_local_tm(tp);
    std::ostringstream oss;
    oss << std::put_time(&local_tm, pattern);
    return oss.str();
}