/**
 * @file line.h
 * @brief Data structures and functions for handling bus lines, stops within lines,
 *        and file processing related to line definitions.
 */
#pragma once
#include "utils.h"
#include "stop.h"

/**
 * @brief Represents a stop on a line.
 */
struct LineStop
{
    std::shared_ptr<Stop> stop;
    int minutes_from_terminus;

    LineStop() : stop(), minutes_from_terminus(0) {}
    LineStop(const std::shared_ptr<Stop> &stop, int minutes_from_terminus)
        : stop(stop), minutes_from_terminus(minutes_from_terminus) {}

    bool operator==(const LineStop &other) const
    {
        return stop == other.stop && minutes_from_terminus == other.minutes_from_terminus;
    }
    bool operator!=(const LineStop &other) const
    {
        return !(*this == other);
    }
};

/**
 * @brief Represents a single line.
 */
class Line
{
private:
    int line_num;                /**< The line's identification number. */
    std::string type;            /**< The type of the line (e.g., "Bus", "Tram", etc.). */
    std::vector<LineStop> stops; /**< The stops on the line, in order. */
public:
    Line() : line_num(0), type("Generikus járat"), stops() {}
    Line(int line_num, const std::string &type = "Generikus járat") : line_num(line_num), type(type), stops() {}
    virtual ~Line() = default;

    virtual const std::string get_type() const { return type; }
    virtual std::shared_ptr<Line> clone() const { return std::make_shared<Line>(*this); }

    const Line &get() const { return *this; }
    const int get_line_num() const { return line_num; }
    const std::vector<LineStop> &get_stops() const { return stops; }

    bool add_stop(const std::shared_ptr<Stop> &stop, int minutes_from_terminus);
    void clear_stops();
    size_t stop_count() const;
    const LineStop &get_stop(size_t index) const;
    const LineStop &get_last_stop() const;

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
    Bus(int line_num) : Line(line_num, "Bus") {}

    std::shared_ptr<Line> clone() const override { return std::make_shared<Bus>(*this); }
};

class Tram : public Line
{
public:
    Tram() : Line() {}
    Tram(int line_num) : Line(line_num, "Tram") {}

    std::shared_ptr<Line> clone() const override { return std::make_shared<Tram>(*this); }
};

class Train : public Line
{
public:
    Train() : Line() {}
    Train(int line_num) : Line(line_num, "Train") {}

    std::shared_ptr<Line> clone() const override { return std::make_shared<Train>(*this); }
};

class HEV : public Line
{
public:
    HEV() : Line() {}
    HEV(int line_num) : Line(line_num, "HEV") {}

    std::shared_ptr<Line> clone() const override { return std::make_shared<HEV>(*this); }
};

class LineManager
{
private:
    std::vector<std::shared_ptr<Line>> linelist; /**< The list of lines. */
public:
    LineManager() : linelist() {}
    const std::vector<std::shared_ptr<Line>> &get_linelist() const { return linelist; }
    const std::shared_ptr<Line> operator[](size_t index) const
    {
        if (index >= linelist.size())
            return nullptr;
        return linelist[index];
    }
    bool process(const std::string &filepath, const StopManager &stopManager);
    bool write_to_file(const std::string &filepath) const;
    void list() const;
    void list_line(int line_num) const;
    size_t find_index(const Line &line) const;
    bool contains(const Line &line) const;
    const std::shared_ptr<Line> get_line_by_number(int line_num) const;
    bool add(const Line &line);
    bool remove(const Line &line);
};