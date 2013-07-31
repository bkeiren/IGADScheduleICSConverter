#include "functions.h"
#include <vector>
#include <sstream>
#include <iostream>

#include <libsicsc.h>

int main( int _Argc, char* _Argv[] )
{
	if (_Argc == 1 || (_Argc >= 2 && strcmp(_Argv[1], "0") == 0))	// UI mode.
	{
		if (_Argc == 1)
		{
			DisplayCommandLineFormat();
			std::cout << "\n! Application will now enter UI mode.\n";
		}

		do
		{
			std::string classcode;
			RequestClassCode(classcode);

			int year;
			RequestStartYear(year);

			int startweek, endweek;
			RequestWeekRange(startweek, endweek);

			ProcessSchedule(classcode, year, startweek, endweek);
		}
		while (1);
	}
	else if (strcmp(_Argv[1], "1") == 0)	// Command line mode.
	{
		std::string classcode = _Argv[2];
		int year = atoi(_Argv[3]);
		int startweek = atoi(_Argv[4]);
		int endweek = atoi(_Argv[5]);

		ProcessSchedule(classcode, year, startweek, endweek);
	}
	else if (strcmp(_Argv[1], "2") == 0)
	{
		int year = atoi(_Argv[3]);
		int startweek = atoi(_Argv[4]);
		int endweek = atoi(_Argv[5]);

		int maxClass = 15;	// Yes, 15. Looooots of IGD classes.
		int maxYear = 4;
		static const std::string disciplines[4] = {"PR", "VA", "DP", "%20IGD"};	// Note the space before IGD! (Silly NHTV people...).
		std::cout	<< "> Application opened in auto mode. Classes 1 thru " 
					<< maxClass 
					<< " for years 1 thru " 
					<< maxYear 
					<< " will be attempted to be parsed for the following displines:\n";
		for (int i = 0; i < 4; ++i)
		{
			if (i != 0)
			{
				std::cout << ", ";
			}
			std::cout << disciplines[i];
		}
		std::cout << std::endl;
		for (int y = 1; y <= maxYear; ++y)
		{
			for (int c = 1; c <= maxClass; ++c)
			{
				for (int d = 0; d < 4; ++d)
				{
					std::stringstream ss;
					ss << y << "GA-" << c << disciplines[d];
					ProcessSchedule(ss.str(), year, startweek, endweek);
				}
			}
		}	
	}
	else
	{
		DisplayCommandLineFormat();
	}

	return 0;
}
