#include "route_manager.h"

void RouteManager::rebuild()
{
	connections.clear();

	for (const std::shared_ptr<Line> &line : lineManager.get_linelist())
	{
		if (!line)
			continue;

		const std::vector<LineStop> &stops = line->get_stops();
		if (stops.size() < 2)
			continue;

		for (size_t i = 1; i < stops.size(); i++)
		{
			const std::shared_ptr<Stop> from_stop = stopManager.get_stop_by_name(stops[i - 1].stop->get_name());
			const std::shared_ptr<Stop> to_stop = stopManager.get_stop_by_name(stops[i].stop->get_name());
			if (!from_stop || !to_stop)
				continue;

			const int travel_time_minutes = stops[i].minutes_from_terminus - stops[i - 1].minutes_from_terminus;
			if (travel_time_minutes < 0)
				continue;

			add(from_stop, to_stop, line, travel_time_minutes);
		}
	}
}

size_t RouteManager::find_index(const std::shared_ptr<Stop> &from, const Connection &conn) const
{
	if (!from || !conn)
		return std::string::npos;

	std::unordered_map<const Stop *, std::vector<Connection>>::const_iterator map_it = connections.find(from.get());
	if (map_it == connections.end())
		return std::string::npos;

	const std::vector<Connection> &stop_connections = map_it->second;
	for (size_t i = 0; i < stop_connections.size(); i++)
	{
		if (stop_connections[i] == conn)
			return i;
	}

	return std::string::npos;
}

bool RouteManager::contains(const std::shared_ptr<Stop> &from, const Connection &conn) const
{
	if (!from || !conn)
		return false;

	std::unordered_map<const Stop *, std::vector<Connection>>::const_iterator map_it = connections.find(from.get());
	if (map_it == connections.end())
		return false;

	const std::vector<Connection> &vec = map_it->second;
	return std::find(vec.begin(), vec.end(), conn) != vec.end();
}

bool RouteManager::add(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::shared_ptr<Line> &line, int travel_time_minutes)
{
	if (!from || !to || !line)
		return false;
	if (!stopManager.contains(*from) || !stopManager.contains(*to) || !lineManager.contains(*line))
		return false;
	if (travel_time_minutes < 0)
		return false;

	Connection conn{to, line, travel_time_minutes};
	if (contains(from, conn))
		return false;

	connections[from.get()].push_back(conn);
	return true;
}

bool RouteManager::remove(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::shared_ptr<Line> &line)
{
	if (!from || !to || !line)
		return false;

	std::unordered_map<const Stop *, std::vector<Connection>>::iterator map_it = connections.find(from.get());
	if (map_it == connections.end())
		return false;

	std::vector<Connection> &vec = map_it->second;
	Connection target_conn{to, line, 0};
	std::vector<Connection>::iterator vec_it = std::find(vec.begin(), vec.end(), target_conn);
	if (vec_it == vec.end())
		return false;

	vec.erase(vec_it);
	if (vec.empty())
		connections.erase(map_it);

	return true;
}

std::vector<Connection> RouteManager::find_connections_by_nodestop(const std::shared_ptr<Stop> &stop) const
{
	if (!stop)
		return {};
	std::unordered_map<const Stop *, std::vector<Connection>>::const_iterator map_it = connections.find(stop.get());
	if (map_it == connections.end())
		return {};
	return map_it->second;
}

std::vector<Connection> RouteManager::find_connections_by_connectionstop(const std::shared_ptr<Stop> &stop) const
{
	if (!stop)
		return {};
	std::vector<Connection> result;
	for (const std::pair<const Stop *const, std::vector<Connection>> &pair : connections)
	{
		const std::vector<Connection> &stop_connections = pair.second;
		for (const Connection &conn : stop_connections)
		{
			if (*conn.get_stop() == *stop)
				result.push_back(conn);
		}
	}
	return result;
}

std::vector<Connection> RouteManager::find_connections_by_line(const std::shared_ptr<Line> &line) const
{
	if (!line)
		return {};
	std::vector<Connection> result;
	for (const std::pair<const Stop *const, std::vector<Connection>> &pair : connections)
	{
		const std::vector<Connection> &stop_connections = pair.second;
		for (const Connection &conn : stop_connections)
		{
			if (*conn.get_line() == *line)
				result.push_back(conn);
		}
	}
	return result;
}

/*
bool RouteManager::write_to_file(const std::string &filepath) const
{
	if (connections.empty())
		return false;

	std::ofstream file(filepath);
	if (!file.is_open())
		return false;

	for (const std::pair<const Stop *, std::vector<Connection>> &pair : connections)
	{
		const Stop *from_stop = pair.first;
		const std::vector<Connection> &stop_connections = pair.second;
		for (const Connection &conn : stop_connections)
		{
			file << from_stop->get_name() << ';'
				 << conn.get_line()->get_line_num() << ';'
				 << conn.get_stop()->get_name() << '#'
				 << conn.get_travel_time_minutes() << std::endl;
		}
	}

	file.close();
	return true;
}
*/