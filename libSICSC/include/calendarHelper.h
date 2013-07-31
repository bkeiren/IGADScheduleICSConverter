#pragma once

#ifndef CALENDARHELPER_H
#define CALENDARHELPER_H

bool GetDayAndMonthFromWeekInYear(int *year, int weekInYear, int *month, int *dayInMonth);

enum EGregorianMonth
{
    EGregorianMonth_January = 1,
    EGregorianMonth_February = 2,
    EGregorianMonth_March = 3,
    EGregorianMonth_April = 4,
    EGregorianMonth_May = 5,
    EGregorianMonth_June = 6,
    EGregorianMonth_July = 7,
    EGregorianMonth_August = 8,
    EGregorianMonth_September = 9,
    EGregorianMonth_October = 10,
    EGregorianMonth_November = 11,
    EGregorianMonth_December = 12
};

/*!
\brief Check if leap year
\par Algorithm
Checks if the year is divisible by 400 or by 4
\code
    if year is divisible by 400 then
       is_leap_year
    else if year is divisible by 100 then
       not_leap_year
    else if year is divisible by 4 then
       is_leap_year
    else
       not_leap_year
\endcode
\returns \c True if a leap year or \c False if not a leap year
\see http://en.wikipedia.org/wiki/Leap_year#Algorithm
*/
bool IsALeapYear(int year);

/*!
\brief Get number of days in a year in a Gregorian calendar
*/
int GetDaysInYear(int year);

/*!
\brief Get number of days in a month according to the Gregorian calendar
*/
int GetDaysInMonth(int year, int month);

/*!
\brief Gets the month and day in month from a day in year
\param year [in] Gregorian year
\param dayInYear [in] Gregorian day on year (1-365 or 1-366 i leap year)
\param month [out] Gregorian month
\param dayInMonth [out] Gregorian day in month
\returns \c True if success or \c False if error
*/
bool GetDayAndMonthFromDayInYear(int year, int dayInYear, int *month, int *dayInMonth);

/*!
\brief Get the number of weeks in a year
\par Algorithm
There are 52 weeks in most years however, years that begin on a Thursday and leap years that begin on a Wednesday have 53 weeks.                
*/
int GetNumberOfWeeksInYear(int year);

/*!
\brief Get the day of week in a Gregorian calendar
\par Algorithm
Uses the Claus Tøndering algorithm
\see http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#T.C3.B8ndering.27s_algorithm
*/
int GetDayOfTheWeek(int year, int month, int day);

#endif