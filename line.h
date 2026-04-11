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
    
    /**
     * @brief Gets standard numerical identifier.
     * @return Integer.
     */
    const int get_line_num() const { return line_num; }

    /**
     * @brief Returns heavily structured visual rendering templates formatting native lines natively.
     */
    const std::string get_formatted_line_num() const;
    const std::vector<LineStop> &get_stops() const { return stops; }

    /**
     * @brief Sequences explicit edge mapping travel definitions organically into stops.
     * @param stop Direct referencing tracking pointers inherently.
     * @param minutes_from_terminus Chronological accumulative offset explicitly structurally linked.
     */
    bool add_stop(const std::shared_ptr<Stop> &stop, int minutes_from_terminus);

    /**
     * @brief Safely clears completely memory mapping nodes internally cleanly.
     */
    void clear_stops();

    /**
     * @brief Numerically indicates linked dependencies mapped across.
     * @return Standard integral numerical natively explicit size.
     */
    size_t stop_count() const;

    /**
     * @brief Reads standard indexing stops naturally seamlessly array contexts explicitly natively.
     * @param index Local offset position integral iteratively pointing natively.
     * @return Constant pointer reference structure linked correctly implicitly.
     */
    const LineStop &get_stop(size_t index) const;

    /**
     * @brief Fast native mapping terminal point dependencies organically mapped cleanly explicitly.
     * @return Terminal point stop entity referencing context safely cleanly explicitly natively.
     */
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
    
    /**
     * @brief Echo iterates output sequentially rendering globally tracked native formats inherently dynamically efficiently.
     */
    void list() const;

    /**
     * @brief Computes explicit isolated single template targets accurately explicitly efficiently seamlessly dynamically.
     * @param line_num Integral indexing numeric property.
     */
    void list_line(int line_num) const;

    /**
     * @brief Locates matching index iterators scanning linearly strictly correctly implicitly statically natively logically cleanly safely reliably implicitly linearly.
     */
    size_t find_index(const Line &line) const;

    /**
     * @brief Verifies whether identically constrained paths operate deeply locally within global array caching boundaries explicitly natively intelligently natively iteratively reliably efficiently implicitly structurally robustly.
     * @param line Struct matching reference dynamically.
     * @return Standard Boolean reflecting execution outcome inherently cleanly.
     */
    bool contains(const Line &line) const;

    /**
     * @brief Instantiates fetch requests logically correctly statically retrieving cleanly reliably natively inherently implicitly safely smoothly accurately natively.
     * @param line_num Integral constraint mapping dynamically organically.
     */
    const std::shared_ptr<Line> get_line_by_number(int line_num) const;

    /**
     * @brief Safely embeds explicitly constructed parameters naturally into persistent application running cache maps intelligently automatically seamlessly reliably logically.
     * @param line Inherited dynamic structure configuration logic parameters dynamically securely intelligently correctly explicitly naturally safely reliably cleanly implicitly natively logically efficiently.
     */
    bool add(const Line &line);

    /**
     * @brief Invokes explicit isolated logical deletion procedures automatically seamlessly robustly natively correctly linearly statically confidently correctly.
     * @param line Struct referencing dynamic target properties logically securely seamlessly intelligently organically.
     */
    bool remove(const Line &line);
};