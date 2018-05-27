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
	struct tm *timeinfo;
	char buffer[80];

	//#pragma warning(suppress: 4996)
	timeinfo = localtime(&rawUnixTime);

	//std::string YYYYmmddHHMMSS = "%Y-%m-%d %I:%M:%S";
	std::string YYYYmmdd = "%Y/%m/%d";

	strftime(buffer, sizeof(buffer), YYYYmmdd.c_str(), timeinfo);
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

//Natural human method
std::string edad(std::string src) {
	//Info about mktime/1  http://www.cplusplus.com/reference/ctime/mktime/

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

#endif // !Herramientas_h


