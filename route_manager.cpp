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

const std::vector<Connection> RouteManager::find_connections_by_connectionstop(const std::shared_ptr<Stop> &stop) const
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

const std::vector<Connection> RouteManager::find_routes(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::chrono::system_clock::time_point &departure_time) const
{
	if (!from || !to)
		return {};

	const std::vector<std::shared_ptr<Schedule>> &all_schedules = scheduleManager.get_schedulelist();
	std::unordered_map<const Line *, std::vector<std::chrono::system_clock::time_point>> line_departures;

	for (size_t i = 0; i < all_schedules.size(); i++)
	{
		line_departures[all_schedules[i]->get_line().get()].push_back(all_schedules[i]->get_start());
	}

	for (std::unordered_map<const Line *, std::vector<std::chrono::system_clock::time_point>>::iterator it = line_departures.begin(); it != line_departures.end(); ++it)
	{
		std::sort(it->second.begin(), it->second.end());
	}

	std::priority_queue<std::pair<int, const Stop *>, std::vector<std::pair<int, const Stop *>>, std::greater<std::pair<int, const Stop *>>> pq;
	std::unordered_map<const Stop *, int> best_times;
	std::unordered_map<const Stop *, std::pair<const Stop *, Connection>> came_from;

	pq.push({0, from.get()});
	best_times[from.get()] = 0;

	while (!pq.empty())
	{
		int current_time = pq.top().first;
		const Stop *current_stop = pq.top().second;
		pq.pop();

		if (*current_stop == *to)
			break;

		if (current_time > best_times[current_stop])
			continue;

		std::unordered_map<const Stop *, std::vector<Connection>>::const_iterator map_it = connections.find(current_stop);
		if (map_it == connections.end())
			continue;

		for (const Connection &conn : map_it->second)
		{
			const Stop *neighbor = conn.get_stop().get();
			const Line *route_line = conn.get_line().get();

			int offset_from_terminus = -1;
			const std::vector<LineStop> &line_stops = route_line->get_stops();
			for (size_t i = 0; i < line_stops.size(); i++)
			{
				if (*line_stops[i].stop == *current_stop)
				{
					offset_from_terminus = line_stops[i].minutes_from_terminus;
					break;
				}
			}

			if (offset_from_terminus == -1 || line_departures.find(route_line) == line_departures.end())
				continue;

			std::chrono::system_clock::time_point needed_terminus_departure = departure_time + std::chrono::minutes(current_time - offset_from_terminus);

			const std::vector<std::chrono::system_clock::time_point> &deps = line_departures[route_line];
			std::vector<std::chrono::system_clock::time_point>::const_iterator next_bus_it = std::lower_bound(deps.begin(), deps.end(), needed_terminus_departure);

			if (next_bus_it == deps.end())
				continue;

			std::chrono::system_clock::time_point bus_leaves_stop_at = *next_bus_it + std::chrono::minutes(offset_from_terminus);

			int wait_time = std::chrono::duration_cast<std::chrono::minutes>(bus_leaves_stop_at - (departure_time + std::chrono::minutes(current_time))).count();
			int new_time = current_time + wait_time + conn.get_travel_time_minutes();

			if (best_times.find(neighbor) == best_times.end() || new_time < best_times[neighbor])
			{
				best_times[neighbor] = new_time;
				came_from[neighbor] = {current_stop, conn};
				pq.push({new_time, neighbor});
			}
		}
	}

	if (came_from.find(to.get()) == came_from.end())
		return {};

	std::vector<Connection> result;
	const Stop *curr = to.get();
	while (*curr != *from)
	{
		result.push_back(came_from[curr].second);
		curr = came_from[curr].first;
	}

	std::reverse(result.begin(), result.end());

	return result;
}