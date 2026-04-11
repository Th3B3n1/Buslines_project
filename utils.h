/**
 * @file utils.h
 * @brief Utility structures and helper functions for date, time and input operations.
 */

#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <chrono>
#include <string>
#include <memory>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include <iomanip>
#include <queue>

namespace utils
{
    /**
     * @brief Parses a time string (e.g. HH:MM) into a total minutes value.
     * @param time_str The time string to parse.
     * @return The parsed total minutes, or -1 if invalid.
     */
    int parse_minutes(const std::string &time_str);

    /**
     * @brief Formats total minutes into a formatted HH:MM string.
     * @param total_minutes Total minutes to format.
     * @return A string formatted as HH:MM.
     */
    std::string format_minutes(int total_minutes);

    /**
     * @brief Converts a standard time_point to the local timezone.
     * @param tp Absolute point in time to convert.
     * @return A std::tm struct with local time values.
     */
    std::tm to_local_tm(const std::chrono::system_clock::time_point &tp);       

    /**
     * @brief Formats a time_point with a strftime-like pattern.
     * @param tp The absolute time point to print.
     * @param pattern The formatting pattern string (e.g., "%Y-%m-%d").
     * @return The formatted time string.
     */
    std::string format_date(const std::chrono::system_clock::time_point &tp, const char *pattern);
}