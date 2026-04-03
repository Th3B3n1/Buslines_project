#include "route_manager.h"

int RouteManager::process(const std::string &filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return -2;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
		std::stringstream ss(unprocessed);
		std::string line_num_str;
		std::string next_str;
		std::getline(ss, line_num_str, ';');
		std::shared_ptr<Line> line = lineManager.get_line_by_number(std::stoi(line_num_str));
		
		while (std::getline(ss, next_str, ';'))
		{
			std::string stop_name_str;
			std::string travel_time_str;
			std::stringstream ss2(next_str);
			std::getline(ss2, stop_name_str, '#');
			std::getline(ss2, travel_time_str, '#');

			std::shared_ptr<Stop> stop = stopManager.get_stop_by_name(stop_name_str);
			int travel_time_minutes = std::stoi(travel_time_str);

			add(stop, line, travel_time_minutes);
		}
	}
	file.close();
	return 0;
}

int RouteManager::find_index(const Connection &conn) const
{
	if (connections.empty())
		return -2;
	if (!conn)
		return -2;
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i] == conn)
		{
			return i;
		}
	}
	return -1;
}

bool RouteManager::contains(const Connection &conn) const
{
	if (connections.empty())
		return false;
	if (!conn)
		return false;
	for (int i = 0; i < connections.size(); i++)
	{
		if (connections[i] == conn)
		{
			return true;
		}
	}
	return false;
}

int RouteManager::add(const std::shared_ptr<Stop> &stop, const std::shared_ptr<Line> &line, int travel_time_minutes)
{
	if (!stop || !line)
		return -2;
	if (!stopManager.contains(*stop) || !lineManager.contains(*line))
		return -1;
	if (contains({stop, line, travel_time_minutes}))
		return -1;
	connections.push_back({stop, line, travel_time_minutes});
	return 0;
}

int RouteManager::remove(const std::shared_ptr<Stop> &stop, const std::shared_ptr<Line> &line)
{
	if (!stop || !line)
		return -2;
	if (!stopManager.contains(*stop) || !lineManager.contains(*line))
		return -1;
	int index = find_index({stop, line, 0});
	if (index == -1)
		return -1;
	connections.erase(connections.begin() + index);
	return 0;
}

/*
int write_lines_to_file(const LineList *linelist, const char *filepath)
{
	if (!linelist)
		return -2;
	std::ofstream file(filepath);
	if (!file.is_open())
		return -2;
	for (int i = 0; i < linelist->count; i++)
	{
		file << linelist->lines[i].line_num << ";" << linelist->lines[i].version;
		for (int y = 0; y < linelist->lines[i].stoplist->count; y++)
		{
			file << ";" << linelist->lines[i].stoplist->stops[y].stop->name << "#" << std::setfill('0') << std::setw(2) << linelist->lines[i].stoplist->stops[y].time_from_terminus.hours << ":" << std::setfill('0') << std::setw(2) << linelist->lines[i].stoplist->stops[y].time_from_terminus.minutes;
		}
		file << "\n";
	}
	file.close();
	return 0;
}
*/