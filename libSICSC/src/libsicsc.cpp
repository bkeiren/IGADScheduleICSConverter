#include "libsicsc.h"

#include <sstream>
#include <iostream>
#include <list>
#include <fstream>
#include <vector>

#include "calendarHelper.h"

#include <windows.h>

namespace
{

struct iCalData
{
	std::string m_Start;
	std::string m_End;
	int m_Day;
	std::string m_Course;
	std::string m_Location;
	std::string m_Lecturer;
	std::list<int> m_CalendarWeeks;
};

// Example: http://brochures.nhtv.nl/roosters/english/bottomtekst.asp?opleiding=igad&rooster=1GA-4PR&week=6
std::string DownloadPage( const std::string& _Class, int _StartWeek, int _EndWeek )
{
	std::stringstream ss;
	ss << "http://194.171.178.25:8080/Reporting/Textspreadsheet;Student%20Sets;name;" << _Class << "?&weeks=" << _StartWeek << "-" << _EndWeek << "&days=1-5&width=0&height=0";
	std::string url = ss.str();

	std::cout << "> URL:\n" << url << std::endl;

	ss.str(std::string(""));	// ss.clear() does not do this.
	ss << "output//output_weeks" << _StartWeek << "-" << _EndWeek << ".html";
	std::string file = ss.str();

	ss.str(std::string(""));
	ss << "curl.exe --url \"" << url << "\" -o \"" << file << "\"";
	std::string cmd = ss.str();

	std::cout << "> curl.exe returned: " << system(cmd.c_str()) << std::endl;

	return file;
}

void ClearDownloadedFiles()
{
	std::cout << "> Deleting intermediate files..." << std::endl;
	system("Del /F /Q /S \"output_weeks*.html\"");
}

bool ConvertHTMLToiCalString( const std::string& _HTMLFile, std::string& _iCalOutput, int _StartYear )
{
	// Read file and store it in a string.
	std::fstream fs;
	fs.open(_HTMLFile.c_str(), std::ios_base::in);
	if (!fs.is_open())
	{
		std::cout << "! Failed to open HTML file '" << _HTMLFile << std::endl;
		return false;
	}

	std::istreambuf_iterator<char> eos;
	std::string* str = new std::string(std::istreambuf_iterator<char>(fs), eos);


	// Check if the page length is less than a certain threshold, since the page that is returned for error code 400 (incorrect classcode)
	// is very short.
	if (str->length() < 200)
	{
		delete str;
		return false;
	}

	size_t t;
	size_t t2;
	// Remove everything that is not within the <body> and </body> tags.
	t = str->find("<body>");
	if (t != str->npos) str->erase(0, t + 6);
	t = str->find("</body>");
	if (t != str->npos) str->erase(t);

	// Remove the first table which contains useless stuff.
	t = str->find("<table");
	t2 = t2 = str->find("</table>");
	if (t != str->npos && t2 != str->npos) str->erase(t, t2 + 8);
	t = str->find("</table>");
	if (t != str->npos) str->erase(0, t + 8);

	// Remove the last table which contains useless stuff.
	t = str->rfind("<table");
	if (t != str->npos) str->erase(t);
	t = str->rfind("<table");
	if (t != str->npos) str->erase(t);

	std::vector<iCalData> iCalDataVector;
	int iteration = 0;
	while (str->length() > 1 && iteration++ < 5)	// > 1 due to character 10 (new line feed) being present occasionally.
	{
		// Erase '<p><span >'
		str->erase(0, 12);
		std::string day = str->substr(0, str->find("</span>"));

		int d = -1;
		if (!day.compare("Monday"))
		{
			d = 0;
		}
		else if (!day.compare("Tuesday"))
		{
			d = 1;
		}
		else if (!day.compare("Wednesday"))
		{
			d = 2;
		}
		else if (!day.compare("Thursday"))
		{
			d = 3;
		}
		else if (!day.compare("Friday"))
		{
			d = 4;
		}
		else
		{
			throw std::exception("Unhandled day of the week!");
		}

		// Substring the table part out of the string.
		int tablestart = str->find("<table");
		int tableend = str->find("</table>");
		std::string table = str->substr(tablestart, tableend - tablestart);
		str->erase(0, tableend + 8);	// Erase '</table>' from the end of str.
		//table.erase()

		// Now remove from the table up until the end of the first row ('</tr>'). (The first table row contains the
		// column titles which we don't need)
		int firstSlashTr = table.find("</tr>");
		if (firstSlashTr == -1) continue;
		table.erase(0, firstSlashTr + 5);

		// Now parse what's left in table for each <tr> </tr> pair. Each pair indicates one appointment (class) in any number of calendarweeks.
		while (table.length() > 1)	// > 1 due to character 10 (new line feed) being present occasionally.
		{
			iCalData data;
			data.m_Day = d;


			table.erase(0, 5);
			int i = table.find("</tr>");
			std::string appointment = table.substr(0, i);
			table.erase(0, i + 5);

			// There should now be 6 lines left in appointment, each one containing specific data.

#define ERASE_TD		{appointment.erase(0, 5);}
#define ERASE_SLASHTD	{int j = appointment.find("</td>");appointment.erase(0, j + 5);}

			ERASE_TD
				data.m_Start = appointment.substr(0, appointment.find("</td>"));
			{
				int k = data.m_Start.find(':');
				data.m_Start.erase(k, 1);
				if (k == 1)
				{
					data.m_Start = "0" + data.m_Start;
				}
			}

			ERASE_SLASHTD

				ERASE_TD
				data.m_End = appointment.substr(0, appointment.find("</td>"));
			{
				int k = data.m_End.find(':');
				data.m_End.erase(k, 1);
				if (k == 1)
				{
					data.m_End = "0" + data.m_End;
				}
			}
			ERASE_SLASHTD

				ERASE_TD
				data.m_Course = appointment.substr(0, appointment.find("</td>"));
			ERASE_SLASHTD

				ERASE_TD
				data.m_Location = appointment.substr(0, appointment.find("</td>"));
			ERASE_SLASHTD

				ERASE_TD
				data.m_Lecturer = appointment.substr(0, appointment.find("</td>"));
			ERASE_SLASHTD

				ERASE_TD
				std::string week = appointment.substr(0, appointment.find("</td>"));
			// Format week.
			bool IsInWeekRangeMode = false;
			int WeekRangeStart = 0;
			while (week.length() > 1)	// > 1 due to character 10 (new line feed) being present occasionally.
			{
				size_t firstNotOf = week.find_first_not_of("0123456789");
				std::string number = week.substr(0, firstNotOf);
				week.erase(0, firstNotOf);	// Erase the number.
				int numberInt = atoi(number.c_str());
				if (IsInWeekRangeMode)
				{
					for (int i = WeekRangeStart; i <= numberInt; ++i)
					{
						data.m_CalendarWeeks.push_back(i);
					}
					week.erase(0, 2);	// Erase ', '.
					IsInWeekRangeMode = false;
				}
				else
				{
					if (firstNotOf == week.npos || week[0] == ',')	// week[0] because we erased the number already.
					{
						data.m_CalendarWeeks.push_back(numberInt);
						week.erase(0, 2);	// Erase ', '.
					}
					else if (week[0] == '-')
					{
						IsInWeekRangeMode = true;
						WeekRangeStart = numberInt;
						week.erase(0, 1);	// Erase '-'.
					}
					else
					{
						std::cout	<< "! Unexpected character found during week parsing (Character: " 
							<< week[0] 
						<< " [" 
							<< (int)week[0] 
						<< "])" 
							<< std::endl;
					}
				}
			}
			ERASE_SLASHTD

#undef ERASE_TD
#undef ERASE_SLASHTD


				iCalDataVector.push_back(data);	
		}
	}

	// At this point, iCalDataVector has been filled with the data that we need. We can now start writing the iCal string.
	std::stringstream iCalSS;

	iCalSS	<< "BEGIN:VCALENDAR" << "\n"
			<< "VERSION:2.0" << "\n"
			<< "PRODID:BKEIRENSCHOOLSCHEDULEAPP" << "\n";


	for (std::vector<iCalData>::iterator it = iCalDataVector.begin(); it != iCalDataVector.end(); ++it)
	{
		iCalData& data = (*it);

		for (std::list<int>::iterator it = data.m_CalendarWeeks.begin(); it != data.m_CalendarWeeks.end(); ++it)
		{
			int actualWeek = (*it);
			int year = actualWeek > 32 ? _StartYear : _StartYear + 1;
			int month = 0;
			int monthday = 0;

			GetDayAndMonthFromWeekInYear(&year, actualWeek, &month, &monthday);
			monthday += data.m_Day;
			int daysInMonth = GetDaysInMonth(year, month);
			if (monthday > daysInMonth)
			{
				monthday -= daysInMonth;
				++month;
				if (month >= 12)
				{
					month = 0;
					++year;
				}
			}

			std::stringstream monthString;
			if (month < 10)
			{
				monthString << 0 << month;	// Add a 0 to make the number two characters wide.
			}
			else
			{
				monthString << month;
			}
			std::stringstream monthdayString;
			if (monthday < 10)
			{
				monthdayString << 0 << monthday;	// Add a 0 to make the number two characters wide.
			}
			else
			{
				monthdayString << monthday;
			}
			std::string mstr = monthString.str();
			std::string mdstr = monthdayString.str();
			iCalSS	<< "BEGIN:VEVENT" << "\n"
				<< "UID:" << year << actualWeek << data.m_Day << data.m_Start << data.m_End << "\n"
				<< "SUMMARY;ENCODING=QUOTED-PRINTABLE:" << data.m_Course << "\n" 
				<< "DESCRIPTION;ENCODING=QUOTED-PRINTABLE:" << data.m_Location << "\n"
				<< "LOCATION;ENCODING=QUOTED-PRINTABLE:" << data.m_Location << "\n"
				<< "DTSTART;TZID=\"Europe/Amsterdam\":" << year << monthString.str() << monthdayString.str() << "T" << data.m_Start << "00" << "\n"
				<< "DTEND;TZID=\"Europe/Amsterdam\":" << year << monthString.str() << monthdayString.str() << "T" << data.m_End << "00" << "\n"
				<< "END:VEVENT" << "\n";
		}
	}

	iCalSS << "END:VCALENDAR" << "\n";

	delete str;

	_iCalOutput = iCalSS.str();

	return true;
}

void StoreiCalAsFile( const std::string& _File, std::string& _iCal )
{
	std::ofstream iCalOS;
	iCalOS.open(_File.c_str());

	if (iCalOS.is_open())
	{
		iCalOS.clear();

		iCalOS << _iCal;

		iCalOS.close();

		std::cout << "> iCal file was saved at:\n" << _File << std::endl;
	}
	else
	{
		std::cout << "! Failed to open output file stream '" << _File << "'. No .ics file output." << std::endl;
	}
}

}


SICSCAPI bool ProcessSchedule( std::string& _ClassCode, int _StartYear, int _StartWeek, int _EndWeek )
{
	std::string classcode = _ClassCode;
	int year = _StartYear;
	int startweek = _StartWeek;
	int endweek = _EndWeek;
	bool res = false;


	if (CreateDirectoryA("output", NULL) != 0)	// Attempt to create the output directory just in case it doesn't exist yet.
	{
		std::cout << "Created output folder 'output'" << std::endl;
	}

	std::string file = DownloadPage(classcode, startweek, endweek);

	std::string ical;
	if (ConvertHTMLToiCalString(file, ical, year))
	{
		std::stringstream iCalFile;
		std::string filename = classcode;
		size_t percentTwenty = filename.find("%20");
		if (percentTwenty != filename.npos)
		{
			// Replace '%20' with '_'.
			std::string fixedFilename = filename.substr(0, percentTwenty);
			filename.erase(0, percentTwenty + 3);
			fixedFilename += "_";
			fixedFilename += filename;
			filename = fixedFilename;
		}
		iCalFile << "output\\" << filename << ".ics";
		StoreiCalAsFile(iCalFile.str(), ical);

		std::cout << "> Done.\n-----------------------------" << std::endl;
		res = true;
	}
	else
	{
		std::cout << "! Failed to construct iCal file from HTML source (This is likely due to an incorrect classcode)." << std::endl;
		res = false;
	}

	ClearDownloadedFiles();
	return res;
}