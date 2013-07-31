/*

	CODE TAKEN FROM http://stackoverflow.com/a/15705378.

*/

#include "calendarHelper.h"

/** ISO number of days in a year */
#define ISO_DAYS_A_YEAR 365

/** ISO number of days in a leap year */
#define ISO_DAYS_A_LEAP_YEAR 366

/** Number of ISO days a week */
#define ISO_DAYS_A_WEEK 7

/** Number of ISO days a week */
#define ISO_MONTHS_A_YEAR 12

/** ISO number of days in February */
#define ISO_DAYS_IN_FEBRUARY 28

/** ISO number of days in February in a leap year */
#define ISO_LEAP_YEAR_DAYS_IN_FEBRUARY 29

/** First month number in a ISO year */
#define ISO_FIRST_MONTH_IN_YEAR 1

/*!
\brief Get the year, month, and day in month from a week in year
\details An ISO week-numbering year has 52 or 53 full weeks (364 or 371 days)
\param year [in, out] Gregorian year
\param weekInYear [in] Gregorian week in a year
\param month [out] Gregorian month
\param dayInMonth [out] Gregorian day in month
\returns \c True if success or \c False if error
*/
bool GetDayAndMonthFromWeekInYear(int *year, int weekInYear, int *month, 
    int *dayInMonth) 
{
    int daysInMonth;
     int weeksInYear;
    *month = ISO_FIRST_MONTH_IN_YEAR;
    bool hasChanged;

    do
    {
        hasChanged = false;

        weeksInYear = GetNumberOfWeeksInYear(*year);
        if (weekInYear > weeksInYear)
        {
            weekInYear -= weeksInYear;
            (*year)++;
            hasChanged = true;
        }

    } while(hasChanged);

    int dayInYear = (weekInYear - 1)  * ISO_DAYS_A_WEEK + 1;

    // Since the first day of week 1 in a year in the Gregorian calendar is not usually January 1st we need to handle the offset
    static int t[] = {0, 0, -1, -2, -3, 3, 2, 1};
    int jan1DayOfWeek = GetDayOfTheWeek(*year, 1, 1); 
    dayInYear += t[jan1DayOfWeek];

    if (dayInYear <= 0)
    {
        // dayInYear is in the previous year
        (*year)--;
        dayInYear += GetDaysInYear(*year);
    }
    else
    {
        int daysInYear = GetDaysInYear(*year);
        if (dayInYear > daysInYear)
        {
            // dayInYear is in the next year
            (*year)++;
            dayInYear -= daysInYear;
        }
    }

    if (!GetDayAndMonthFromDayInYear(*year, dayInYear, month, dayInMonth))
        return false;

    return true;
}

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
bool IsALeapYear(int year) 
{
    return (!(year % 4) && (year % 100)) || !(year % 400);
}

/*!
\brief Get number of days in a year in a Gregorian calendar
*/
int GetDaysInYear(int year) 
{
    return IsALeapYear(year) ? ISO_DAYS_A_LEAP_YEAR : ISO_DAYS_A_YEAR;
}

/*!
\brief Get number of days in a month according to the Gregorian calendar
*/
int GetDaysInMonth(int year, int month) 
{
    int daysInMonth;

    switch (month)
    {
        case EGregorianMonth_February:
            daysInMonth = IsALeapYear(year) ? ISO_LEAP_YEAR_DAYS_IN_FEBRUARY : ISO_DAYS_IN_FEBRUARY;
            break;
        case EGregorianMonth_April:
        case EGregorianMonth_June:
        case EGregorianMonth_September:
        case EGregorianMonth_November:
            daysInMonth = 30;
            break;
        default:
            daysInMonth = 31;
            break;
    }

    return daysInMonth;
}

/*!
\brief Gets the month and day in month from a day in year
\param year [in] Gregorian year
\param dayInYear [in] Gregorian day on year (1-365 or 1-366 i leap year)
\param month [out] Gregorian month
\param dayInMonth [out] Gregorian day in month
\returns \c True if success or \c False if error
*/
bool GetDayAndMonthFromDayInYear(int year, int dayInYear, int *month, 
    int *dayInMonth) 
{
    int daysInMonth;
    *month = ISO_FIRST_MONTH_IN_YEAR;

    for (int i = ISO_FIRST_MONTH_IN_YEAR; i <= ISO_MONTHS_A_YEAR ; i++)
    {
        daysInMonth = GetDaysInMonth(year, i);

        if (dayInYear <= daysInMonth) break;

        (*month)++;
        dayInYear -= daysInMonth;
    }

    *dayInMonth = (int)dayInYear;

    return true;
}

/*!
\brief Get the number of weeks in a year
\par Algorithm
There are 52 weeks in most years however, years that begin on a Thursday and leap years that begin on a Wednesday have 53 weeks.                
*/
int GetNumberOfWeeksInYear(int year) 
{
    int jan1DayOfWeek = GetDayOfTheWeek(year, 1, 1);                    
    return jan1DayOfWeek == 4 || (jan1DayOfWeek == 3 && IsALeapYear(year)) ? 53 : 52;
}

/*!
\brief Get the day of week in a Gregorian calendar
\par Algorithm
Uses the Claus Tøndering algorithm
\see http://en.wikipedia.org/wiki/Calculating_the_day_of_the_week#T.C3.B8ndering.27s_algorithm
*/
int GetDayOfTheWeek(int year, int month, int day) 
{
   static int t[] = {0, 3, 2, 5, 0, 3, 5, 1, 4, 6, 2, 4};
   year -= month < 3;
   return (year + year/4 - year/100 + year/400 + t[month-1] + day) % 7;
}