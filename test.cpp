#include "gtest_lite.h"
#include "utils.h"
#include "route_manager.h"
#include "schedule.h"
#include "line.h"
#include "stop.h"

#ifdef CPORTA
int main()
{
    TEST(UtilsTest, ParseMinutesNormal)
        EXPECT_EQ(750, utils::parse_minutes("12:30"));
        EXPECT_EQ(0, utils::parse_minutes("00:00"));
        EXPECT_EQ(1439, utils::parse_minutes("23:59"));
    END

    TEST(UtilsTest, ParseMinutesInvalidEdges)
        EXPECT_EQ(-1, utils::parse_minutes(""));
        EXPECT_EQ(0, utils::parse_minutes("0"));
        EXPECT_EQ(60, utils::parse_minutes("60"));
    END

    TEST(UtilsTest, FormatMinutesNormal)
        EXPECT_STREQ("12:30", utils::format_minutes(750).c_str());
        EXPECT_STREQ("00:00", utils::format_minutes(0).c_str());
        EXPECT_STREQ("23:59", utils::format_minutes(1439).c_str());
    END

    TEST(UtilsTest, FormatMinutesEdgeCases)
        // Negative value defaults to "00:00" based on implementation
        EXPECT_STREQ("00:00", utils::format_minutes(-10).c_str());
        EXPECT_STREQ("25:00", utils::format_minutes(1500).c_str());
        EXPECT_STREQ("01:05", utils::format_minutes(65).c_str());
    END

    TEST(RouteManagerTest, DirectRoute)
        StopManager sm;
        LineManager lm;
        ScheduleManager schedm;

        auto sA = std::make_shared<Stop>("A_Stop");
        auto sB = std::make_shared<Stop>("B_Stop");
        sm.add(*sA);
        sm.add(*sB);

        Bus line(100);
        line.add_stop(sm.get_stop_by_name("A_Stop"), 0);
        line.add_stop(sm.get_stop_by_name("B_Stop"), 15);
        lm.add(line);

        std::tm tm_sched = {};
        tm_sched.tm_year = 2025 - 1900;
        tm_sched.tm_mon = 0; // Jan
        tm_sched.tm_mday = 1;
        tm_sched.tm_hour = 8;
        tm_sched.tm_min = 0;
        auto sched_time = std::chrono::system_clock::from_time_t(std::mktime(&tm_sched));

        Schedule sched(lm.get_line_by_number(100), sched_time, sm.get_stop_by_name("B_Stop"));
        schedm.add(sched);

        RouteManager rm(sm, lm, schedm);
        rm.rebuild();

        auto routes = rm.find_routes(sm.get_stop_by_name("A_Stop"), sm.get_stop_by_name("B_Stop"), sched_time);

        EXPECT_EQ((size_t)1, routes.size());
        if (routes.size() > 0)
        {
            EXPECT_EQ(15, routes[0].get_travel_time_minutes());
            EXPECT_STREQ("B_Stop", routes[0].get_stop()->get_name().c_str());
        }
    END

    TEST(RouteManagerTest, WaitTimeRequired)
        StopManager sm;
        LineManager lm;
        ScheduleManager schedm;

        auto sA = std::make_shared<Stop>("A_Stop");
        auto sB = std::make_shared<Stop>("B_Stop");
        sm.add(*sA);
        sm.add(*sB);

        Bus line(100);
        line.add_stop(sm.get_stop_by_name("A_Stop"), 0);
        line.add_stop(sm.get_stop_by_name("B_Stop"), 15);
        lm.add(line);

        std::tm tm_sched = {};
        tm_sched.tm_year = 2025 - 1900;
        tm_sched.tm_mon = 0;
        tm_sched.tm_mday = 1;
        tm_sched.tm_hour = 8;
        tm_sched.tm_min = 0;
        auto sched_time = std::chrono::system_clock::from_time_t(std::mktime(&tm_sched));

        Schedule sched(lm.get_line_by_number(100), sched_time, sm.get_stop_by_name("B_Stop"));
        schedm.add(sched);

        RouteManager rm(sm, lm, schedm);
        rm.rebuild();

        // 10 minutes early arrival, should wait and reach B.
        std::tm tm_req = tm_sched;
        tm_req.tm_hour = 7;
        tm_req.tm_min = 50;
        auto req_time = std::chrono::system_clock::from_time_t(std::mktime(&tm_req));

        auto routes = rm.find_routes(sm.get_stop_by_name("A_Stop"), sm.get_stop_by_name("B_Stop"), req_time);

        EXPECT_EQ((size_t)1, routes.size());
    END

    TEST(RouteManagerTest, UnreachableRoute)
        StopManager sm;
        LineManager lm;
        ScheduleManager schedm;

        auto sA = std::make_shared<Stop>("A_Stop");
        auto sB = std::make_shared<Stop>("B_Stop");
        auto sC = std::make_shared<Stop>("C_Unreachable");
        sm.add(*sA);
        sm.add(*sB);
        sm.add(*sC);

        Bus line(100);
        line.add_stop(sm.get_stop_by_name("A_Stop"), 0);
        line.add_stop(sm.get_stop_by_name("B_Stop"), 15);
        lm.add(line);

        std::tm tm_sched = {};
        tm_sched.tm_year = 2025 - 1900;
        tm_sched.tm_mon = 0;
        tm_sched.tm_mday = 1;
        tm_sched.tm_hour = 8;
        tm_sched.tm_min = 0;
        auto sched_time = std::chrono::system_clock::from_time_t(std::mktime(&tm_sched));

        Schedule sched(lm.get_line_by_number(100), sched_time, sm.get_stop_by_name("B_Stop"));
        schedm.add(sched);

        RouteManager rm(sm, lm, schedm);
        rm.rebuild();

        auto routes = rm.find_routes(sm.get_stop_by_name("A_Stop"), sm.get_stop_by_name("C_Unreachable"), sched_time);

        EXPECT_EQ((size_t)0, routes.size());
    END

    TEST(StopManagerTest, ProcessFile)
        std::ofstream off("temp_stops.txt");
        off << "Keleti palyaudvar\nBudaPart\nEde utca\n\n";
        off.close();

        StopManager sm;
        bool success = sm.process("temp_stops.txt");
        EXPECT_TRUE(success);
        EXPECT_EQ((size_t)3, sm.get_stoplist().size());
        EXPECT_TRUE(sm.get_stop_by_name("Keleti palyaudvar") != nullptr);
        EXPECT_TRUE(sm.get_stop_by_name("Ede utca") != nullptr);
        
        std::remove("temp_stops.txt");
    END

    TEST(StopManagerTest, ProcessFileFailure)
        StopManager sm;
        bool success = sm.process("non_existent_stops.txt");
        EXPECT_FALSE(success);
    END

    TEST(LineManagerTest, ProcessFile)
        StopManager sm;
        sm.add(Stop("A_Stop"));
        sm.add(Stop("B_Stop"));

        std::ofstream off("temp_lines.txt");
        // format: line_num;type;stop1#minutes;stop2#minutes
        off << "100;Bus;A_Stop#0;B_Stop#15\n"; // Valid
        off << "200;Tram;A_Stop#0;Fake_Stop#20\n"; // Invalid stop (Fake_Stop not in StopManager)
        off.close();

        LineManager lm;
        bool success = lm.process("temp_lines.txt", sm);
        EXPECT_TRUE(success);
        
        // Only one line should be successfully added because Fake_Stop doesn't exist
        EXPECT_EQ((size_t)1, lm.get_linelist().size());
        if(lm.get_linelist().size() > 0)
        {
            EXPECT_EQ(100, lm.get_linelist()[0]->get_line_num());
            EXPECT_STREQ("Bus", lm.get_linelist()[0]->get_type().c_str());
            EXPECT_EQ((size_t)2, lm.get_linelist()[0]->get_stops().size());
        }

        std::remove("temp_lines.txt");
    END

    TEST(LineManagerTest, ProcessFileFailure)
        StopManager sm;
        LineManager lm;
        bool success = lm.process("non_existent_lines.txt", sm);
        EXPECT_FALSE(success);
    END

    TEST(ScheduleManagerTest, ProcessFile)
        StopManager sm;
        sm.add(Stop("A_Stop"));
        sm.add(Stop("B_Stop"));

        LineManager lm;
        Bus line(100);
        line.add_stop(sm.get_stop_by_name("A_Stop"), 0);
        line.add_stop(sm.get_stop_by_name("B_Stop"), 15);
        lm.add(line);

        std::ofstream off("temp_schedules.txt");
        // format: line_num;last_stop;YYYY-MM-DD HH:MM:SS
        off << "100;B_Stop;2025-01-01 08:00:00\n"; // Valid
        off << "999;B_Stop;2025-01-01 09:00:00\n"; // Invalid line
        off << "100;Fake_Stop;2025-01-01 10:00:00\n"; // Invalid stop
        off << "100;A_Stop;Bad_Date_Format\n"; // Invalid date (skipped or won't parse properly)
        off.close();

        ScheduleManager schedm;
        bool success = schedm.process("temp_schedules.txt", lm, sm);
        EXPECT_TRUE(success);

        // Only the first schedule should be valid and loaded
        EXPECT_EQ((size_t)1, schedm.get_schedulelist().size());
        if(schedm.get_schedulelist().size() > 0)
        {
            EXPECT_EQ(100, schedm.get_schedulelist()[0]->get_line()->get_line_num());
            EXPECT_STREQ("B_Stop", schedm.get_schedulelist()[0]->get_last_stop()->get_name().c_str());
        }
        
        std::remove("temp_schedules.txt");
    END

    TEST(ScheduleManagerTest, ProcessFileFailure)
        StopManager sm;
        LineManager lm;
        ScheduleManager schedm;
        bool success = schedm.process("non_existent_schedules.txt", lm, sm);
        EXPECT_FALSE(success);
    END

    return 0;
}
#endif