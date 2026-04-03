#include "line.h"

bool LineManager::process(const std::string &filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return false;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
		std::stringstream ss(unprocessed);
		std::string line_num_str;
		std::string type_str;

		std::getline(ss, line_num_str, ';');
		std::getline(ss, type_str, ';');
		if (type_str == "Bus")
		{
			Bus bus = Bus(std::stoi(line_num_str));
			add(bus);
		}
		else if (type_str == "Tram")
		{
			Tram tram = Tram(std::stoi(line_num_str));
			add(tram);
		}
		else if (type_str == "Train")
		{
			Train train = Train(std::stoi(line_num_str));
			add(train);
		}
		else if (type_str == "HEV")
		{
			HEV hev = HEV(std::stoi(line_num_str));
			add(hev);
		}
		else
		{
			Line line = Line(std::stoi(line_num_str));
			add(line);
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
		file << linelist[i]->get_line_num() << std::endl;
	}
	file.close();
	return true;
}

void LineManager::list() const
{
	for (size_t i = 0; i < linelist.size(); i++)
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Jarat: " << linelist[i]->get_line_num() << std::endl;
		std::cout << "Tipus: " << linelist[i]->get_type() << std::endl;
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
			std::cout << "Jarat: " << linelist[i]->get_line_num() << std::endl;
			std::cout << "Tipus: " << linelist[i]->get_type() << std::endl;
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
	if (index < 0)
	{
		return nullptr;
	}
	return linelist[index];
}

bool LineManager::add(const Line &line)
{
	if (linelist.empty())
		return false;
	if (contains(line))
		return false;
	linelist.push_back(std::make_shared<Line>(line));
	return true;
}

bool LineManager::remove(const Line &line)
{
	if (linelist.empty())
		return false;
	size_t lineindex = find_index(line);
	if (lineindex == std::string::npos)
		return false;
	linelist.erase(linelist.begin() + lineindex);
	return true;
}