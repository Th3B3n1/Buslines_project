#pragma once
#include "utils.h"
#include "stop.h"
#include "line.h"
#include "schedule.h"

class Connection
{
private:
    std::shared_ptr<Stop> stop;
    std::shared_ptr<Line> line;
    int travel_time_minutes;

public:
    Connection() : stop(nullptr), line(nullptr), travel_time_minutes(0) {}
    Connection(const std::shared_ptr<Stop> &stop, const std::shared_ptr<Line> &line, int travel_time_minutes)
        : stop(stop), line(line), travel_time_minutes(travel_time_minutes) {}
    const Connection &get() const { return *this; }
    const std::shared_ptr<Stop> &get_stop() const { return stop; }
    const std::shared_ptr<Line> &get_line() const { return line; }
    const int get_travel_time_minutes() const { return travel_time_minutes; }
    bool operator==(const Connection &other) const
    {
        if (!stop || !line || !other.stop || !other.line)
            return false;
        return *stop == *other.stop && *line == *other.line;
    }
    bool operator!=(const Connection &other) const
    {
        return !(*this == other);
    }
    operator bool() const
    {
        return stop != nullptr && line != nullptr;
    }
};

class RouteManager
{
private:
    std::unordered_map<const Stop *, std::vector<Connection>> connections;
    StopManager &stopManager;
    LineManager &lineManager;
    ScheduleManager &scheduleManager;

public:
    RouteManager(StopManager &sm, LineManager &lm, ScheduleManager &schedm) : connections(), stopManager(sm), lineManager(lm), scheduleManager(schedm) {}
    void rebuild();
    size_t find_index(const std::shared_ptr<Stop> &from, const Connection &conn) const;
    bool contains(const std::shared_ptr<Stop> &from, const Connection &conn) const;
    bool add(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::shared_ptr<Line> &line, int travel_time_minutes);
    bool remove(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::shared_ptr<Line> &line);
    const std::vector<Connection> find_connections_by_connectionstop(const std::shared_ptr<Stop> &stop) const;
    const std::vector<Connection> find_routes(const std::shared_ptr<Stop> &from, const std::shared_ptr<Stop> &to, const std::chrono::system_clock::time_point &departure_time) const;
};