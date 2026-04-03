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

    void stop_menu();
    void line_menu();
    void schedule_menu();
    void find_routes();
    void route_menu();
    void save();
    void save_menu();

public:
    Menu() : routes(stops, lines) {
        stops.process("megallok.txt");
        lines.process("jaratok.txt");
        schedules.process("menetrendek.txt");
        routes.process("jaratok.txt");
    }
    void run();
    void exit();
};