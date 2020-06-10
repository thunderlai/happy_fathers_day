#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <functional>
#include <array>
#include "assert.h"

enum class Month : int {
    January,
    February,
    March,
    April,
    May,
    June,
    July,
    August,
    September,
    October,
    November,
    December
}; //C++11 scoped and based enum

struct date {
    int month;
    int day;
    int year;
};

template <int D, int M, int Y>
struct DayOfTheWeek {
    constexpr static const int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
    static int const y = Y - (M < 3);
    static int const value = (y + y / 4 - y / 100 + y / 400 + t[M - 1] + D) % 7;
};

template <int Y>
struct FathersDayForAGivenYear {
    static int const day = (DayOfTheWeek<1, 6, Y>::value == 0 ? 1 : 8 - DayOfTheWeek<1, 6, Y>::value) + 14;
};


//constexpr int TABLE_SIZE = 10;
constexpr int TABLE_SIZE = 50;

/**
 * Variadic template for a recursive helper struct.
 */
template<int INDEX = 0, int ...D>
//struct Helper : Helper<INDEX + 1, D..., INDEX * INDEX> { };
struct Helper : Helper<INDEX + 1, D..., FathersDayForAGivenYear<INDEX + 2020>::day > { };

/**
 * Specialization of the template to end the recursion when the table size reaches TABLE_SIZE.
 */
template<int ...D>
struct Helper<TABLE_SIZE, D...> {
    static constexpr std::array<int, TABLE_SIZE> table = { D... };
};

constexpr std::array<int, TABLE_SIZE> table = Helper<>::table;



namespace via_ctime {
    std::string get_day_from_number(uint32_t n)
    {
        // could turn this into an enum class
        const std::vector<std::string> days = {
            "Sunday",
            "Monday",
            "Tuesday",
            "Wednesday",
            "Thursday",
            "Friday",
            "Saturday"
        };

        uint32_t i = n % 7;
        if (i >= days.size())
            return "Could not determine the day...";

        return days[i];
    }

    int get_day_from_date(std::tm time_in)
    {
        std::time_t time_temp = std::mktime(&time_in);
        const std::tm* time_out = std::localtime(&time_temp);
        int day = time_out->tm_wday;
        return day;
    }

    // function which takes a year and tells the date which father's day falls on
    std::tm get_fathers_day_for_year(int year) {
        // figure out the day of june 1st for that year
        assert(year >= 1900);
        if (year <= 1900)
            year = 1900;
        std::tm time_in = { 0,0,0, 1, 5, year - 1900 };
        uint32_t june_first_day = get_day_from_date(time_in);
        assert(june_first_day >= 0 && june_first_day < 7);

        //figure out first sunday in june
        int date_day_of_first_sunday_in_june = june_first_day == 0 ? 1 : 7 - june_first_day + 1;
        int date_day_of_third_sunday_in_june = date_day_of_first_sunday_in_june + 14;

        std::tm time_out = { 0,0,0, date_day_of_third_sunday_in_june, 5, year - 1900 };

        assert(get_day_from_date(time_out) == 0);
        return time_out;
    }

    int GetFathersDay(int year) {
        return get_fathers_day_for_year(year).tm_mday;
    }

    date get_current_date() {
        time_t now = std::time(0);

        tm* today = std::localtime(&now);
        int month = today->tm_mon;
        int mday = today->tm_mday;
        int year = today->tm_year + 1900;
        return {month, mday, year};
    }
}

namespace sakamoto {
    int dayofweek(int d, int m, int y)
    {
        static int t[] = { 0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4 };
        y -= m < 3;
        return (y + y / 4 - y / 100 + y / 400 + t[m - 1] + d) % 7;
    }

    int GetFathersDay(int year)
    {
        auto first_of_june_day = dayofweek(1, 6, year);
        return (first_of_june_day == 0 ? 1 : 7 - first_of_june_day + 1) + 14;
    }
}

namespace tmpl_mp {
    int GetFathersDay(int year) {
        // year must be 2020 to 2069, if not... we didn't precalculate it
        if (year >= 2020 && year <= 2069) {
            size_t index = year - 2020;
            assert(index < table.size());
            if (index < table.size()) {
                return table[index];
            }
        }
        return ~0;
    }
}


int main()
{
    std::vector<std::function<int(int)>> vFuncs = {
        via_ctime::GetFathersDay,
        sakamoto::GetFathersDay,
        tmpl_mp::GetFathersDay
    };


    date today = via_ctime::get_current_date();
    std::cout << "Today is " << static_cast<int>(today.month) + 1 << "/" << today.day << "/" << today.year << std::endl;

    int fathers_mday_result = ~0;

    // sanity check
    int year = today.year;

    for (int i = 0; i < 50; i++) {
        int prev_result = 0;
        std::cout << year + i << std::endl;
        for (auto func : vFuncs) {
            fathers_mday_result = func(year+i);
            if (prev_result != 0 && prev_result != fathers_mday_result) {
                assert(prev_result == fathers_mday_result);
            }
            prev_result = fathers_mday_result;
            std::cout << "   June " << fathers_mday_result << std::endl;
        }
        std::cout << std::endl;
    }

    if (today.month == static_cast<int>(Month::June) && today.day == fathers_mday_result)
        std::cout << "Happy Father's Day!!!"  << std::endl;
    else {
        std::cout << "Not Father's Day yet..." << std::endl;
    }

    return 0;
}
