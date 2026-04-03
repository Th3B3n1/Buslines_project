/**
 * @file stop.h
 * @brief Data structures and operations for handling stops and stop lists.
 */
#pragma once
#include "utils.h"

/**
 * @brief Represents a single bus stop.
 */
class Stop
{
private:
    std::string name; /**< Name of the stop. */
public:
    Stop() : name("") {}
    Stop(const std::string &name) : name(name) {}
    const Stop& get() { return *this; }
    const std::string &get_name() const { return name; }
    bool operator==(const Stop &other) const
    {
        return name == other.name;
    }
    bool operator!=(const Stop &other) const
    {
        return !(*this == other);
    }
    operator bool() const
    {
        return !name.empty();
    }
};

class StopManager
{
private:
    std::vector<std::shared_ptr<Stop>> stoplist; /**< List of all stops. */
public:
    StopManager() : stoplist() {}
    StopManager(const std::vector<std::shared_ptr<Stop>> &stops) : stoplist(stops) {}
    const std::vector<std::shared_ptr<Stop>> &get_stoplist() const { return stoplist; }
    const std::shared_ptr<Stop> operator[](int index) const
    {
        if (index < 0 || index >= stoplist.size())
            return nullptr;
        return stoplist[index];
    }
    /**
     * @brief Loads stops from a text file into the stop list.
     *
     * The file must contain one stop name per line.
     *
     * @param filepath Path to the file containing the stop names.
     * @return 0 on success, -2 on file or memory error.
     */
    int process(const std::string &filepath);

    /**
     * @brief Writes all stops in the list to a file, one per line.
     *
     * @param filepath Output file path.
     * @return 0 on success, -2 on file error.
     */
    int write_to_file(const std::string &filepath) const;

    /**
     * @brief Prints all stops in the list to stdout.
     *
     * Each stop name is printed on a separate line.
     */
    void list() const;

    int find_index(const std::string &stopname) const;

    /**
     * @brief Checks if a stop exists in the list.
     *
     * Performs a linear search through the list.
     *
     * @param stop Stop to search for.
     * @return True if the stop is found, false otherwise.
     */
    bool contains(const Stop &stop) const;

    const std::shared_ptr<Stop> get_stop_by_name(const std::string &stopname) const;

    /**
     * @brief Adds a new stop to the list.
     *
     * The stopname is stored directly without copying. Caller must ensure it remains valid.
     *
     * @param stopname String containing the stop name. Must be valid until removed from list.
     * @return 0 on success, -1 if already exists, -2 if error.
     */
    int add(const std::string &stopname);

    /**
     * @brief Removes a stop from the list by name.
     *
     * @param stopname Name of the stop to remove.
     * @return 0 on success, -1 if not found, -2 if error.
     */
    int remove(const std::string &stopname);
};