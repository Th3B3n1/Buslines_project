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

int parse_minutes(const std::string &time_str);
std::string format_minutes(int total_minutes);