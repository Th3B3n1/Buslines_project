#pragma once
#include "utils.h"
#include "stop.h"
#include "line.h"
#include "schedule.h"

class Menu
{
private:
    StopManager stops;
    LineManager lines;
    ScheduleManager schedules;

    void stop_menu();
    void line_menu();
    void schedule_menu();
    void route_menu();
    void save_menu();

public:
    Menu() {}
    void run();
    void exit();
};