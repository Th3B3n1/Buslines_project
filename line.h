/**
 * @file line.h
 * @brief Data structures and functions for handling bus lines, stops within lines,
 *        and file processing related to line definitions.
 */
#pragma once
#include "utils.h"
#include "stop.h"

/**
 * @brief Represents a single line.
 */
class Line
{
private:
    int line_num; /**< The line's identification number. */
public:
    Line() : line_num(0) {}
    Line(int line_num) : line_num(line_num) {}
    virtual ~Line() = default;
    virtual const std::string get_type() const { return "Generikus járat"; }
    const Line& get() { return *this; }
    const int get_line_num() const { return line_num; }
    bool operator==(const Line &other) const
    {
        return line_num == other.line_num;
    }
    bool operator!=(const Line &other) const
    {
        return !(*this == other);
    }
    operator bool() const
    {
        return line_num > 0;
    }
};

class Bus : public Line
{
public:
    Bus() : Line() {}
    Bus(int line_num) : Line(line_num) {}

    const std::string get_type() const override { return "Bus"; }
};

class Tram : public Line
{
public:
    Tram() : Line() {}
    Tram(int line_num) : Line(line_num) {}

    const std::string get_type() const override { return "Tram"; }
};

class Train : public Line
{
public:
    Train() : Line() {}
    Train(int line_num) : Line(line_num) {}

    const std::string get_type() const override { return "Train"; }
};

class HEV : public Line
{
public:
    HEV() : Line() {}
    HEV(int line_num) : Line(line_num) {}

    const std::string get_type() const override { return "HEV"; }
};

class LineManager
{
private:
    std::vector<std::shared_ptr<Line>> linelist; /**< The list of lines. */
public:
    LineManager() : linelist() {}
    const std::vector<std::shared_ptr<Line>> &get_linelist() const { return linelist; }
    const std::shared_ptr<Line> &operator[](int index) const
    {
        if (index < 0 || index >= linelist.size())
            return nullptr;
        return linelist[index];
    }
    int process(const std::string &filepath);
    int write_to_file(const std::string &filepath) const;
    void list() const;
    void list_line(int line_num) const;
    int find_index(const Line &line) const;
    bool contains(const Line &line) const;
    const std::shared_ptr<Line> get_line_by_number(int line_num) const;
    int add(const Line &line);
    int remove(const Line &line);
};