#pragma once

#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <string>

void DisplayCommandLineFormat();

void RequestClassCode( std::string& _Output );

void RequestWeekRange( int& _Start, int& _End );

void RequestStartYear( int& _Start );

#endif