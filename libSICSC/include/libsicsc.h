#pragma once

#ifndef LIBSICSC
#define LIBSICSC

#ifdef SICSCEXPORT
	#define SICSCAPI __declspec(dllexport)
#else
	#define SICSCAPI __declspec(dllimport)
#endif

#include <string>

SICSCAPI bool ProcessSchedule( std::string& _ClassCode, int _StartYear, int _StartWeek, int _EndWeek );

#endif