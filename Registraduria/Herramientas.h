#pragma once 

#ifndef Herramientas_h
#define Herramientas_h

#include <fstream>
#include <time.h>			//time(_), localtime_s, srtftime
#include <ctime>			//struct tm	
#include <sstream>

//Parses time using ISO-8601
std::string timeNow() {
	time_t rawUnixTime = time(0);
	struct tm *timeinfo;
	char buffer[80];

#pragma warning(suppress: 4996)
	timeinfo = localtime(&rawUnixTime);

	//std::string YYYYmmddHHMMSS = "%Y-%m-%d %I:%M:%S";
	std::string YYYYmmdd = "%Y/%m/%d";

	strftime(buffer, sizeof(buffer), YYYYmmdd.c_str(), timeinfo);
	std::string str(buffer);

	//std::cout << str;
	return str;
}

void buildTimeStruct(std::tm &tmStruct, const int years, const int months, const int days) {

	tmStruct.tm_year = years - 1900;
	tmStruct.tm_mon = months - 1;
	tmStruct.tm_mday = days;
	tmStruct.tm_hour = 0;   tmStruct.tm_min = 0; tmStruct.tm_sec = 0;

}

//Natural human method
std::string edad(std::string src) {
	//Info about mktime/1  http://www.cplusplus.com/reference/ctime/mktime/

	const std::string delim = "/";
	size_t pos, years, months, days, yearsC, monthsC, daysC;

	std::string today = timeNow();
	pos = today.find(delim, 0);
	std::istringstream(today.substr(0, pos)) >> yearsC;
	today.erase(0, pos + delim.length());
	pos = today.find(delim, 0);
	std::istringstream(today.substr(0, pos)) >> monthsC;
	today.erase(0, pos + delim.length());
	std::istringstream(today) >> daysC;

	//Extract tokens into strings and inyect into integers
	pos = src.find(delim, 0);
	std::istringstream(src.substr(0, pos)) >> years;
	src.erase(0, pos + delim.length());
	pos = src.find(delim, 0);
	std::istringstream(src.substr(0, pos)) >> months;
	src.erase(0, pos + delim.length());
	std::istringstream(src) >> days;
	//dd = src;									//Redundant since src only got days now

	size_t sYears = yearsC - years;

	if (monthsC == months) {
		if (daysC < days) sYears--;
	}
	else if (monthsC < months) {
		sYears--;
	}

	//For debugging----------------------
	//std::stringstream out;
	//out << "\nYears: " << (int)sYears;// << "\tMonths: " << (int)sMonths << "\tDays: " << (int)sDays << '\n';
	//std::string outStr = out.str();
	//std::cout << outStr;

	std::stringstream ssYYYY;
	ssYYYY << sYears;

	return ssYYYY.str();
}

#endif // !Herramientas_h


