/**
 * @file schedule.h
 * @brief Data structures and functions for handling schedules.
 */
#pragma once
#include "utils.h"
#include "line.h"

/**
 * @brief Represents a scheduled bus departure.
 */
class Schedule
{
private:
    std::vector<std::shared_ptr<Line>> line; /**< Pointer to the associated bus line. */
    std::chrono::system_clock::time_point start;                         /**< The starting date and time of the schedule. */
public:
    Schedule() : line(), start() {}
    Schedule(const std::vector<std::shared_ptr<Line>> &line, const std::chrono::system_clock::time_point &start) : line(line), start(start) {}
    const std::vector<std::shared_ptr<Line>> &get_line() const { return line; }
    const std::chrono::system_clock::time_point &get_start() const { return start; }
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
     * @return 0 on success, -2 on error.
     */
    int process(const std::string &filepath);

    /**
     * @brief Writes schedules to a file in the standard format.
     * @param filepath Path to the output file.
     * @return 0 on success, -2 on error.
     */
    int write_to_file(const std::string &filepath) const;

    /**
     * @brief Prints all schedules in the list to stdout.
     */
    void list() const;

    /**
     * @brief Finds the index of a Schedule in a ScheduleList.
     * @param schedule Pointer to the Schedule being searched.
     * @return Index of the schedule, or -1 if not found.
     */
    int find_index(const Schedule &schedule) const;

    bool contains(const Schedule &schedule) const;

    /**
     * @brief Adds a Schedule to a ScheduleList.
     * @param schedule Pointer to the Schedule to add.
     * @return 0 on success, -1 on duplicate, -2 on error.
     */
    int add(const Schedule &schedule);

    /**
     * @brief Removes a Schedule from a ScheduleList.
     * @param schedule Pointer to the Schedule to remove.
     * @return 0 on success, -1 if the schedule is not found, -2 on error.
     */
    int remove(const Schedule &schedule);
};
