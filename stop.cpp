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
		add(unprocessed);
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

size_t StopManager::find_index(const std::string &stopname) const
{
	for (size_t i = 0; i < stoplist.size(); i++)
	{
		if (stoplist[i]->get_name() == stopname)
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
	size_t index = find_index(stopname);
	if (index == std::string::npos)
		return nullptr;
	return stoplist[index];
}

bool StopManager::add(const std::string &stopname)
{
	Stop newstop(stopname);
	if (contains(newstop))
		return false;
	stoplist.push_back(std::make_shared<Stop>(newstop));
	return true;
}

bool StopManager::remove(const std::string &stopname)
{
	size_t stopindex = find_index(stopname);
	if (stopindex == std::string::npos)
		return false;
	stoplist.erase(stoplist.begin() + stopindex);
	return true;
}
