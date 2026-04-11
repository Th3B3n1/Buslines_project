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
    
    /**
     * @brief Access a Stop pointer by vector offset index safely.
     * @param index Target vector offset.
     * @return Shared pointer to Stop. Nullptr if out of bounds.
     */
    const std::shared_ptr<Stop> operator[](size_t index) const
    {
        if (index >= stoplist.size())
            return nullptr;
        return stoplist[index];
    }
    
    /**
     * @brief Loads stops sequentially from a text file database.
     *
     * The file structure expects one valid stop string configuration per line.
     *
     * @param filepath The formatted URI pointing to the configuration map.
     * @return Boolean true upon streaming pipeline success, natively catching errors.
     */
    bool process(const std::string &filepath);

    /**
     * @brief Streams dynamic working cache memory persistently back to storage safely.
     *
     * @param filepath Safely writable output target wrapper.
     * @return Final true validating complete IO write execution successfully.
     */
    bool write_to_file(const std::string &filepath) const;

    /**
     * @brief Dumps active memory stop tables vertically iterating out cleanly across stdout.
     */
    void list() const;

    /**
     * @brief Determines array offset lookup indices.
     * @param stop Direct referencing comparison entity.
     * @return Underlying sequence standard mapping integer length size_t.
     */
    size_t find_index(const Stop &stop) const;

    /**
     * @brief Checks if a distinct logical stop exists redundantly inside caches.
     *
     * Leverages robust standard operators iterating iteratively spanning matching.
     *
     * @param stop Logical referencing structure parameter input.
     * @return True when identical properties map perfectly globally.
     */
    bool contains(const Stop &stop) const;

    /**
     * @brief Dynamically fetches managed instances using standard strict strings.
     * @param stopname Logical matching reference template keyword.
     * @return Allocated shared dynamic caching element tracking Stop instances natively.
     */
    const std::shared_ptr<Stop> get_stop_by_name(const std::string &stopname) const;

    /**
     * @brief Injects new explicitly named Stops locally into global memory contexts natively.
     *
     * @param stop Raw instantiated dynamic object context.
     * @return Validated boolean evaluating duplication rules cleanly natively.
     */
    bool add(const Stop &stop);

    /**
     * @brief Scans active structures and invalidates specific standard entities cleaning caches recursively.
     *
     * @param stop The standard lookup model struct to isolate natively cleanly.
     * @return Truth indicating absolute destruction logic success correctly natively.
     */
    bool remove(const Stop &stop);
};