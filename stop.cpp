#include "stop.h"

bool StopManager::process(const std::string &filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return false;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
		if (unprocessed.empty())
			continue;
		add(Stop(unprocessed));
	}
	file.close();
	return true;
}

bool StopManager::write_to_file(const std::string &filepath) const
{
	if (stoplist.empty())
		return false;
	std::ofstream file(filepath);
	if (!file.is_open())
		return false;
	for (size_t i = 0; i < stoplist.size(); i++)
	{
		file << stoplist[i]->get_name() << std::endl;
	}
	file.close();
	return true;
}

void StopManager::list() const
{
	for (size_t i = 0; i < stoplist.size(); i++)
	{
		std::cout << stoplist[i]->get_name() << std::endl;
	}
}

size_t StopManager::find_index(const Stop &stop) const
{
	for (size_t i = 0; i < stoplist.size(); i++)
	{
		if (*stoplist[i] == stop)
		{
			return i;
		}
	}
	return std::string::npos;
}

bool StopManager::contains(const Stop &stop) const
{
	for (size_t i = 0; i < stoplist.size(); i++)
	{
		if (*stoplist[i] == stop)
		{
			return true;
		}
	}
	return false;
}

const std::shared_ptr<Stop> StopManager::get_stop_by_name(const std::string &stopname) const
{
	size_t index = find_index(Stop(stopname));
	if (index == std::string::npos)
		return nullptr;
	return stoplist[index];
}

bool StopManager::add(const Stop &stop)
{
	if (contains(stop))
		return false;
	stoplist.push_back(std::make_shared<Stop>(stop));
	return true;
}

bool StopManager::remove(const Stop &stop)
{
	size_t stopindex = find_index(stop);
	if (stopindex == std::string::npos)
		return false;
	stoplist.erase(stoplist.begin() + stopindex);
	return true;
}
