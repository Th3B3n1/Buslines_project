/**
 * @file schedule.h
 * @brief Data structures and functions for handling schedules.
 */
#pragma once
#include "utils.h"
#include "line.h"
#include "stop.h"

/**
 * @brief Represents a scheduled bus departure.
 */
class Schedule
{
private:
    std::shared_ptr<Line> line;                  /**< Pointer to the associated bus line. */
    std::chrono::system_clock::time_point start; /**< The starting date and time of the schedule. */
    std::shared_ptr<Stop> last_stop;             /**< Pointer to the last stop of the schedule. */
public:
    Schedule() : line(), start(), last_stop() {}
    Schedule(const std::shared_ptr<Line> &line, const std::chrono::system_clock::time_point &start, const std::shared_ptr<Stop> &last_stop) : line(line), start(start), last_stop(last_stop) {}
    
    /**
     * @brief Reads standard associated linking logically implicitly mapped line organically.
     * @return Underlying shared natively constructed element reliably pointing cleanly explicitly.
     */
    const std::shared_ptr<Line> &get_line() const { return line; }

    /**
     * @brief Evaluates chronologically implicit baseline absolute starting references natively reliably.
     * @return Absolute epoch logic timing structures cleanly safely statically explicitly.
     */
    const std::chrono::system_clock::time_point &get_start() const { return start; }

    /**
     * @brief Computes inherently the boundary destination constraint parameters cleanly explicitly iteratively safely natively.
     * @return Underlying referenced natively shared pointer boundary constraints efficiently correctly implicitly automatically.
     */
    const std::shared_ptr<Stop> &get_last_stop() const { return last_stop; }
    bool operator==(const Schedule &other) const
    {
        if (!line || !other.line)
            return false;
        return *line == *other.line && start == other.start && last_stop == other.last_stop;
    }
    bool operator!=(const Schedule &other) const
    {
        return !(*this == other);
    }
    operator bool() const
    {
        return line != nullptr && start.time_since_epoch().count() > 0;
    }
};

class ScheduleManager
{
private:
    std::vector<std::shared_ptr<Schedule>> schedulelist; /**< The list of schedules. */
public:
    ScheduleManager() : schedulelist() {}
    const std::vector<std::shared_ptr<Schedule>> &get_schedulelist() const { return schedulelist; }
    /**
     * @brief Loads schedules from a file and processes them into a ScheduleList.
     *
     * File format example:
     *   line_num;version;YYYY-MM-DD HH:MM:SS
     *
     * @param filepath Path to the input file.
     * @param linemanager Reference to the line manager.
     * @param stopmanager Reference to the stop manager.
     * @return true on success, false on error.
     */
    bool process(const std::string &filepath, const LineManager &linemanager, const StopManager &stopmanager);

    /**
     * @brief Writes schedules to a file in the standard format.
     * @param filepath Path to the output file.
     * @return true on success, false on error.
     */
    bool write_to_file(const std::string &filepath) const;

    /**
     * @brief Prints all schedules in the list to stdout.
     */
    void list() const;

    /**
     * @brief Finds the index of a Schedule in a ScheduleList.
     * @param schedule Pointer to the Schedule being searched.
     * @return Index of the schedule, or std::string::npos if not found.
     */
    size_t find_index(const Schedule &schedule) const;

    bool contains(const Schedule &schedule) const;

    /**
     * @brief Adds a Schedule to a ScheduleList.
     * @param schedule Pointer to the Schedule to add.
     * @return true on success, false on duplicate or error.
     */
    bool add(const Schedule &schedule);

    /**
     * @brief Removes a Schedule from a ScheduleList.
     * @param schedule Pointer to the Schedule to remove.
     * @return true on success, false if the schedule is not found or on error.
     */
    bool remove(const Schedule &schedule);
};
