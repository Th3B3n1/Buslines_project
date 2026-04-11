#include "schedule.h"

bool ScheduleManager::process(const std::string &filepath, const LineManager &linemanager, const StopManager &stopmanager)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return false;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
		if (unprocessed.empty())
			continue;
		std::stringstream ss(unprocessed);
		std::string line_num_str;
		std::string last_stop_str;
		std::string departure_str;
		if (!std::getline(ss, line_num_str, ';') || !std::getline(ss, last_stop_str, ';') || !std::getline(ss, departure_str, ';'))
		{
			std::cout << "Invalid schedule row: " << unprocessed << std::endl;
			continue;
		}

		std::shared_ptr<Line> line = linemanager.get_line_by_number(std::stoi(line_num_str));
		std::shared_ptr<Stop> stop = stopmanager.get_stop_by_name(last_stop_str);
		if (line == nullptr)
		{
			std::cout << "Line " << line_num_str << " not found. Will skip current schedule." << std::endl;
			continue;
		}
		if (stop == nullptr)
		{
			std::cout << "Stop " << last_stop_str << " not found. Will skip current schedule." << std::endl;
			continue;
		}

		std::tm t = {};
		std::stringstream ss_dep(departure_str);
		ss_dep >> std::get_time(&t, "%Y-%m-%d %H:%M:%S");
		if (ss_dep.fail())
		{
			std::cout << "Invalid departure time for line " << line_num_str << " and stop " << last_stop_str << ". Will skip current schedule." << std::endl;
			continue;
		}

		std::chrono::system_clock::time_point date = std::chrono::system_clock::from_time_t(std::mktime(&t));
		Schedule schedule(line, date, stop);
		add(schedule);
	}
	file.close();
	return true;
}

bool ScheduleManager::write_to_file(const std::string &filepath) const
{
	if (schedulelist.empty())
		return false;
	std::ofstream file(filepath);
	if (!file.is_open())
		return false;
	for (size_t i = 0; i < schedulelist.size(); i++)
	{
		file << schedulelist[i]->get_line()->get_line_num() << ";"
			 << schedulelist[i]->get_last_stop()->get_name() << ";"
			 << utils::format_date(schedulelist[i]->get_start(), "%Y-%m-%d %H:%M:%S") << "\n";
	}
	file.close();
	return true;
}

void ScheduleManager::list() const
{
	for (size_t i = 0; i < schedulelist.size(); i++)
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Jarat: " << schedulelist[i]->get_line()->get_formatted_line_num() << std::endl;
		std::cout << "Vegallomas: " << schedulelist[i]->get_last_stop()->get_name() << std::endl;
		std::cout << "Elso indulas: " << utils::format_date(schedulelist[i]->get_start(), "%Y.%m.%d. %H:%M:%S") << std::endl;
		std::cout << "----------------------" << std::endl;
	}
}

size_t ScheduleManager::find_index(const Schedule &schedule) const
{
	if (!schedule)
		return std::string::npos;
	for (size_t i = 0; i < schedulelist.size(); i++)
	{
		if (*schedulelist[i] == schedule)
		{
			return i;
		}
	}
	return std::string::npos;
}

bool ScheduleManager::contains(const Schedule &schedule) const
{
	if (!schedule)
		return false;
	for (size_t i = 0; i < schedulelist.size(); i++)
	{
		if (*schedulelist[i] == schedule)
		{
			return true;
		}
	}
	return false;
}

bool ScheduleManager::add(const Schedule &schedule)
{
	if (!schedule)
		return false;
	if (contains(schedule))
		return false;
	schedulelist.push_back(std::make_shared<Schedule>(schedule));
	return true;
}

bool ScheduleManager::remove(const Schedule &schedule)
{
	if (schedulelist.empty())
		return false;
	size_t index = find_index(schedule);
	if (index == std::string::npos)
		return false;
	schedulelist.erase(schedulelist.begin() + index);
	return true;
}