/**
 * @file line.h
 * @brief Data structures and functions for handling bus lines, stops within lines,
 *        and file processing related to line definitions.
 */
#pragma once
#include "utils.h"
#include "stop.h"

/**
 * @brief Represents a single bus line.
 */
class Line
{
private:
    int line_num;                   /**< The line's identification number. */
    int version;                    /**< The version of the line. */
public:
    Line() : line_num(0), version(0) {}
    Line(int line_num, int version) : line_num(line_num), version(version) {}
    int get_line_num() const { return line_num; }
    int get_version() const { return version; }
    bool operator==(const Line &other) const
    {
        return line_num == other.line_num && version == other.version;
    }
};

class LineManager
{
private:
    std::vector<std::shared_ptr<Line>> linelist; /**< The list of lines. */
public:
    LineManager() : linelist() {}
    const std::vector<std::shared_ptr<Line>> &get_linelist() const { return linelist; }
    int process(const std::string &filepath);
    int write_to_file(const std::string &filepath) const;
    void list() const;
    void list_line(int line_num, int version) const;
    int find_index(const Line &line) const;
    bool contains(const Line &line) const;
    int add(const Line &line);
    int remove(const Line &line);
};