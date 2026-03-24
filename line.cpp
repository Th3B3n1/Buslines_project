#include "line.h"

int LineManager::process(const std::string &filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return -2;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
        std::stringstream ss(unprocessed);
        std::string line_num_str;
        std::string version_str;

        std::getline(ss, line_num_str, ';');
        std::getline(ss, version_str, ';');
		Line line = Line(std::stoi(line_num_str), std::stoi(version_str));
		linelist.push_back(std::make_shared<Line>(line));
	}
	file.close();
	return 0;
}

int LineManager::write_to_file(const std::string &filepath) const
{
	std::ofstream file(filepath);
	if (!file.is_open())
		return -2;
	for (int i = 0; i < linelist.size(); i++)
	{
		file << linelist[i]->get_line_num() << ";" << linelist[i]->get_version() << std::endl;
	}
	file.close();
	return 0;
}

void LineManager::list() const
{
	if (linelist.empty())
		return;
	for (int i = 0; i < linelist.size(); i++)
	{
		std::cout << "----------------------" << std::endl;
		std::cout << "Jarat: " << linelist[i]->get_line_num() << std::endl;
		std::cout << "Verzio: " << linelist[i]->get_version() << std::endl;
		std::cout << "----------------------" << std::endl;
	}
}

void LineManager::list_line(int line_num, int version) const
{
	if (linelist.empty())
		return;
	for (int i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == Line(line_num, version))
		{
			std::cout << "----------------------" << std::endl;
			std::cout << "Jarat: " << linelist[i]->get_line_num() << std::endl;
			std::cout << "Verzio: " << linelist[i]->get_version() << std::endl;
			std::cout << "----------------------" << std::endl;
			return;
		}
	}
	std::cout << "Nincs ilyen jarat." << std::endl;
}

int LineManager::find_index(const Line &line) const
{
	if (linelist.empty())
		return -2;
	if (line == Line())
		return -2;
	for (int i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == line)
		{
			return i;
		}
	}
	return -1;
}

bool LineManager::contains(const Line &line) const
{
	if (linelist.empty())
		return false;
	if (line == Line())
		return false;
	for (int i = 0; i < linelist.size(); i++)
	{
		if (*linelist[i] == line)
		{
			return true;
		}
	}
	return false;
}

int LineManager::add(const Line &line)
{
	if (linelist.empty())
		return -2;
	if (contains(line))
		return -1;
	linelist.push_back(std::make_shared<Line>(line));
	return 0;
}

int LineManager::remove(const Line &line)
{
	if (linelist.empty())
		return -2;
	int lineindex = find_index(line);
	if (lineindex == -1)
		return -1;
	linelist.erase(linelist.begin() + lineindex);
	return 0;
}