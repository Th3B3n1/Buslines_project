#include "stop.h"

int StopManager::process(const std::string &filepath)
{
	std::ifstream file(filepath);
	if (!file.is_open())
		return -2;
	std::string unprocessed;
	while (std::getline(file, unprocessed))
	{
		if (unprocessed.empty())
			continue;
		add(unprocessed);
	}
	file.close();
	return 0;
}

int StopManager::write_to_file(const std::string &filepath) const
{
	if (stoplist.empty())
		return -2;
	std::ofstream file(filepath);
	if (!file.is_open())
		return -2;
	for (int i = 0; i < stoplist.size(); i++)
	{
		file << stoplist[i]->get_name() << std::endl;
	}
	file.close();
	return 0;
}

void StopManager::list() const
{
	if (stoplist.empty())
		return;
	for (int i = 0; i < stoplist.size(); i++)
	{
		std::cout << stoplist[i]->get_name() << std::endl;
	}
}

int StopManager::find_index(const std::string &stopname) const
{
	if (stoplist.empty())
		return -2;
	for (int i = 0; i < stoplist.size(); i++)
	{
		if (stoplist[i]->get_name() == stopname)
		{
			return i;
		}
	}
	return -1;
}

bool StopManager::contains(const Stop &stop) const
{
	if (stoplist.empty())
		return false;
	for (int i = 0; i < stoplist.size(); i++)
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
	int index = find_index(stopname);
	if (index < 0)
	{
		return nullptr;
	}
	return stoplist[index];
}

int StopManager::add(const std::string &stopname)
{
	if (stoplist.empty())
		return -2;
	Stop newstop(stopname);
	if (contains(newstop))
		return -1;
	stoplist.push_back(std::make_shared<Stop>(newstop));
	return 0;
}

int StopManager::remove(const std::string &stopname)
{
	if (stoplist.empty())
		return -2;
	int stopindex = find_index(stopname);
	if (stopindex == -1)
		return -1;
	stoplist.erase(stoplist.begin() + stopindex);
	return 0;
}
