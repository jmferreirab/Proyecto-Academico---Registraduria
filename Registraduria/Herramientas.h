#pragma once 

#ifndef Herramientas_h
#define Herramientas_h

#include <fstream>
#include <time.h>			//time(_), localtime_s, srtftime
#include <ctime>			//struct tm	
#include <sstream>

//iterators are better for interfacing 
void fixStr(std::string &s, const char &old_value, const char &new_value) {
	std::string::iterator first = s.begin(), last = s.end();
	for (; first != last; first++) {
		if (*first == old_value) *first = new_value;
	}
}

void fixStr(std::string &s) {
	std::string::iterator first = s.begin(), last = s.end();
	for (; first != last; first++) {
		if (*first == '_') *first = ' ';
	}
}

//Parses time using ISO-8601
std::string timeNow() {
	time_t rawUnixTime = time(0);
	struct tm timeinfo;
	char buffer[80];

	localtime_s(&timeinfo, &rawUnixTime);

	//std::string YYYYmmddHHMMSS = "%Y-%m-%d %I:%M:%S";
	std::string YYYYmmdd = "%Y/%m/%d";

	strftime(buffer, sizeof(buffer), YYYYmmdd.c_str(), &timeinfo);
	std::string str(buffer);

	//std::cout << str;
	return str;
}

void convertStringCitytoCityCode(std::ifstream &ifs) {
	//replaceRegex("Bolivar,220);
	//replaceRegex("Bogota",22);
	//replaceRegex("Leticia",223);

	//generar 1122 while(find(cityName)) replace(cityName,cityCode)  to allow the list to be indexed using integers rather than std::string
	//
	//agregar luego de tener los candidatos en memoria principal el campo codigo de ciudad
	//que lleva a 1122 ifs(cityName1) candidato.cityCode = cityCode1...
	//
	//Aun si el txt original tuviera los codigos de ciudad, al insertar un nuevo registro el programa necesitaria contar con la capacidad para usar un mapa o lista de tuplas
	//para llenar el campo codigo de ciudad. Para eso los 1122 ifs vuelven a ser necesarios.

	//Basado en esto se decide pagar el costo de indexar las lista usando como llave/indice una cadena en lugar de un entero.
}

void tokenizeInPlace(std::string &src, const std::string &delim) {

	std::string token;
	int pos;

	while ((pos = src.find(delim, 0)) != std::string::npos) {		//Haga pos = index donde aparece el delimitador, y cheque si dicho index es distinto al fin de la cadena
	token = src.substr(0, pos);
	std::cout << token << std::endl;
		src.erase(0, pos + delim.length());
	}
	std::cout << src << std::endl;
}

void buildTimeStruct(std::tm &tmStruct, const int years, const int months, const int days) {

	tmStruct.tm_year = years - 1900;
	tmStruct.tm_mon = months - 1;
	tmStruct.tm_mday = days;
	tmStruct.tm_hour = 0;   tmStruct.tm_min = 0; tmStruct.tm_sec = 0;

}

//Method using diff in seconds between timestamps and then deducing amount of yyyy mm dd passed from remainder.
int edad1970(std::string src) {

	const std::string delim = "/";
	int pos, years, months, days;

	//Extract tokens into strings and inyect into integers
	pos = src.find(delim, 0);
	std::istringstream(src.substr(0, pos)) >> years;
	src.erase(0, pos + delim.length());
	pos = src.find(delim, 0);
	std::istringstream(src.substr(0, pos)) >> months;
	src.erase(0, pos + delim.length());
	std::istringstream(src) >> days;
	//dd = src;									//Redundant since src only got days now

	//Build a container capable of being parsed to time_t. Parse it.
	std::tm tmStruct;
	buildTimeStruct(tmStruct, years, months, days);
	time_t rr = mktime(&tmStruct);

	time_t timer = time(0);
	
	double seconds = 0;
	if (years>=70) seconds = difftime(timer, mktime(&tmStruct));
	else seconds = difftime(timer, mktime(&tmStruct));
	//difftime(timer,mktime(&tmStruct));

	double toDay = 1.0 / (60 * 60 * 24);
	double toMonth = toDay / 30.5;
	double toYear = (1.0 / (60 * 60 * 24)) / 365.25;

	//how old you are?
	//bdate 1997 03 19
	//cdate 2018 05 20

	//There are 2018-1997 years
	double sYears = seconds * toYear;

	//The integer part of years must be converted to months.
	double sMonths = (sYears - (int)sYears) * 12;

	double sDays = (sMonths - (int)sMonths) * 30.5;

	//For debugging----------------------
	//std::stringstream out;
	//out << "\nYears: " << (int)sYears << "\tMonths: " << (int)sMonths << "\tDays: " << (int)sDays << '\n';
	//std::string outStr = out.str();
	//std::cout << outStr;

	return (int)sYears;

}

//Natural human method
std::string edad(std::string src) {
	const std::string delim = "/";
	int pos, years, months, days, yearsC, monthsC, daysC;
	
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

	int sYears = yearsC - years;
	
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


inline std::vector<std::string> tokenize(const std::string &s, const std::string &delimiter) {
	std::vector<std::string> ret;
	size_t start = 0;
	size_t end = 0;
	size_t len = 0;
	std::string token;
	do {
		end = s.find(delimiter, start);
		len = end - start;
		token = s.substr(start, len);
		ret.emplace_back(token);
		start += len + delimiter.length();
		std::cout << token << std::endl;
	} while (end != std::string::npos);
	return ret;
}


#endif // !Herramientas_h


