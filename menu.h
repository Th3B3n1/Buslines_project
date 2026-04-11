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

    /**
     * @brief Reads user terminal integers locally.
     * @return Standard selection int.
     */
    int get_choice();

    /**
     * @brief Interactive submenu viewing/managing specific Stop entities.
     */
    void stop_menu();

    /**
     * @brief Line/Bus connection management UI wrapper.
     */
    void line_menu();

    /**
     * @brief Processes times scheduled statically against routes.
     */
    void schedule_menu();

    /**
     * @brief The main prompt invoking Dijkstra execution and formatted printouts.
     */
    void route_menu();

    /**
     * @brief Re-exports running memory variables back out into .txt formats.
     */
    void save();

    /**
     * @brief Saves file IO interactive question looping.
     */
    void save_menu();

public:
    Menu() : routes(stops, lines, schedules)
    {
        stops.process("megallok.txt");
        lines.process("jaratok.txt", stops);
        schedules.process("menetrendek.txt", lines, stops);
        routes.rebuild();
    }
    
    /**
     * @brief Initiates system loop UI.
     */
    void run();

    /**
     * @brief Safely shuts down processes.
     */
    void exit();
};