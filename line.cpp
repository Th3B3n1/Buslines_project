#include "line.h"

std::shared_ptr<Line> make_line(const std::string &type, int line_num)
{
	if (type == "Bus")
		return std::make_shared<Bus>(line_num);
	if (type == "Tram")
		return std::make_shared<Tram>(line_num);
	if (type == "Train")
		return std::make_shared<Train>(line_num);
	if (type == "HEV")
		return std::make_shared<HEV>(line_num);
	return std::make_shared<Line>(line_num, type);
}

bool Line::add_stop(const std::shared_ptr<Stop> &stop, int minutes_from_terminus)
{
	if (!stop || minutes_from_terminus < 0)
		return false;
	if (!stops.empty() && minutes_from_terminus < stops.back().minutes_from_terminus)
		return false;
	stops.push_back(LineStop(stop, minutes_from_terminus));
	return true;
}

const std::string Line::get_formatted_line_num() const
{
	std::ostringstream oss;
	int base_linenum = line_num / 10;
	int suffix = (line_num % 10) - 1;
	char suffix_char = (suffix >= 0 && suffix < 26) ? ('A' + suffix) : '?';
	if (suffix > 0)
		oss << base_linenum << suffix_char;
	else
		oss << base_linenum;
	return oss.str();
}

void Line::clear_stops()
{
	stops.clear();
}

size_t Line::stop_count() const
{
	return stops.size();
}

const LineStop &Line::get_stop(size_t index) const
{
	if (index >= stops.size())
		throw std::out_of_range("Index out of bounds");
	return stops[index];
}

const LineStop &Line::get_last_stop() const
{
	if (stops.empty())
		throw std::out_of_range("Line has no stops");
	return stops.back();
}

bool LineManager::process(const std::string &filepath, const StopManager &stopManager)
{
	linelist.clear();
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
		std::string type_str;
		if (!std::getline(ss, line_num_str, ';'))
			continue;
		if (!std::getline(ss, type_str, ';'))
			continue;

		int line_num = std::stoi(line_num_str);
		std::shared_ptr<Line> line = make_line(type_str, line_num);
		bool valid_line = true;

		std::string next_str;
		while (std::getline(ss, next_str, ';'))
		{
			if (next_str.empty())
				continue;
			std::stringstream stop_stream(next_str);
			std::string stop_name_str;
			std::string travel_time_str;
			if (!std::getline(stop_stream, stop_name_str, '#'))
			{
				valid_line = false;
				break;
			}
			if (!std::getline(stop_stream, travel_time_str))
			{
				valid_line = false;
				break;
			}

			int travel_time_minutes = utils::parse_minutes(travel_time_str);
			std::shared_ptr<Stop> stop = stopManager.get_stop_by_name(stop_name_str);
			if (!stop)
			{
				valid_line = false;
				break;
			}
			if (travel_time_minutes < 0 || !line->add_stop(stop, travel_time_minutes))
			{
				valid_line = false;
				break;
			}
		}

		if (valid_line && line->stop_count() > 0)
		{
			add(*line);
		}
	}
	file.close();
	return true;
}

bool LineManager::write_to_file(const std::string &filepath) const
{
	if (linelist.empty())
		return false;
	std::ofstream file(filepath);
	if (!file.is_open())
		return false;

	for (size_t i = 0; i < linelist.size(); i++)
	{
		const std::shared_ptr<Line> &line = linelist[i];
		file << line->get_line_num() << ';' << line->get_type();
		for (const LineStop &LineStop: line->get_stops())
		{
			file << ';' << LineStop.stop->get_name() << '#' << utils::format_minutes(LineStop.minutes_from_terminus);
		}
		file << std::endl;
	}

	file.close();
	return true;
}

void LineManager::list() const
{
	for (size_t i = 0; i < linelist.size(); i++)
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Jarat: " << linelist[i]->get_formatted_line_num() << std::endl;
		std::cout << "Tipus: " << linelist[i]->get_type() << std::endl;
		if (!linelist[i]->get_stops().empty())
		{
			const LineStop &first_stop = linelist[i]->get_stop(0);
			const LineStop &last_stop = linelist[i]->get_last_stop();
			std::cout << "Utvonal: " << first_stop.stop->get_name() << " -> " << last_stop.stop->get_name() << std::endl;
		}
		std::cout << "Megallok szama: " << linelist[i]->stop_count() << std::endl;
		std::cout << "----------------------" << std::endl;
	}
}

void LineManager::list_line(int line_num) const
{
	if (line_num <= 0)
		return;
	bool found = false;
	for (size_t i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == Line(line_num))
		{
			std::cout << "----------------------" << std::endl;
			std::cout << "Jarat: " << linelist[i]->get_formatted_line_num() << std::endl;
			std::cout << "Tipus: " << linelist[i]->get_type() << std::endl;
			for (const LineStop &stop : linelist[i]->get_stops())
			{
				std::cout << stop.stop->get_name() << " # " << utils::format_minutes(stop.minutes_from_terminus) << std::endl;
			}
			std::cout << "----------------------" << std::endl;
			found = true;
		}
	}
	if (!found)
	{
		std::cout << "Nincs ilyen jarat." << std::endl;
	}
}

size_t LineManager::find_index(const Line &line) const
{
	if (!line)
		return std::string::npos;
	for (size_t i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == line)
		{
			return i;
		}
	}
	return std::string::npos;
}

bool LineManager::contains(const Line &line) const
{
	if (!line)
		return false;
	for (size_t i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == line)
		{
			return true;
		}
	}
	return false;
}

const std::shared_ptr<Line> LineManager::get_line_by_number(int line_num) const
{
	size_t index = find_index(Line(line_num));
	if (index == std::string::npos)
	{
		return nullptr;
	}
	return linelist[index];
}

bool LineManager::add(const Line &line)
{
	if (!line)
		return false;
	if (contains(line))
		return false;
	linelist.push_back(line.clone());
	return true;
}

bool LineManager::remove(const Line &line)
{
	if (!line)
		return false;
	size_t lineindex = find_index(line);
	if (lineindex == std::string::npos)
		return false;
	linelist.erase(linelist.begin() + lineindex);
	return true;
}