#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <chrono>
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

int main()
{
    date today = get_current_date();
    std::cout << "Today is " << static_cast<int>(today.month) + 1 << "/" << today.day << "/" << today.year << std::endl;

    int fathers_mday_result = ~0;

    fathers_mday_result = GetFathersDay(today.year);

    if (today.month == static_cast<int>(Month::June) && today.day == fathers_mday_result)
        std::cout << "Happy Father's Day!!!"  << std::endl;
    else {
        std::cout << "Not Father's Day yet..." << std::endl;
    }

}
