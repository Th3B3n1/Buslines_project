#pragma once
#include "utils.h"
#include "stop.h"
#include "line.h"
#include "schedule.h"
#include "route_manager.h"

class Menu
{
private:
    StopManager stops;
    LineManager lines;
    ScheduleManager schedules;
    RouteManager routes;

    int get_choice();
    void stop_menu();
    void line_menu();
    void schedule_menu();
    void route_menu();
    void save();
    void save_menu();

public:
    Menu() : routes(stops, lines, schedules)
    {
        stops.process("megallok.txt");
        lines.process("jaratok.txt", stops);
        schedules.process("menetrendek.txt", lines, stops);
        routes.rebuild();
    }
    void run();
    void exit();
};