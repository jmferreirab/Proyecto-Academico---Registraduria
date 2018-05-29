#pragma once
#include "IndexedList.h"
#include "StructLib.h"
#include <iomanip>
#include <sstream>
#include "Herramientas.h"
#include "AVL.h"

using std::setw;
using std::ostringstream;
using std::istringstream;
using std::string;

///Clase para la administracion para las entidades principales de la simulacion
class Registraduria {

private:
	AVLTree<Candidato>  alcaldes;
	AVLTree<CandidaturaPresidencial>  candidaturasPresidenciales;
	AVLTree<Departamento> cntrDptos;

public:

	Registraduria() {};

	//Version 2. Su parametro paso a tipo Single Sorted Linked List. 
	//Esta funcion empaca los contenidos del archivo siguiendo reglas fijas sobre el formato esperado en el archivo.
	//El parametro es SSLL dado que este tipo de lista es capaz de sortear sobre la clave de tipo string al momento de insertar.
	//Tras ejecutar la funcion, list (dependiendo de la clave que la funcion) elija, estara sorteada por clave.
	//A medida que nuevos candidatos aparecen eston son empacan en la jerarquia compuesta data por AVLTree<Departamento>
	bool obtenerDatosArchivo(const char* filepath = "Data.txt") {
		//std::ifstream file("C:\\Data\\Data.txt");
		std::ifstream file(filepath);

		Candidato candd, vice;
		CandidaturaPresidencial canddPres;

		char delim = '+';

		if (!file)
		{
			std::cerr << "No se pudo leer el archivo de entrada\n";
			//cin.get();
			exit(0);
			return false;

		}

		//It would be a bad idea to directly create the Candidadto when reading from stream. What if we decide to delete the candidadte midway.
		while (!file.eof()) {

			//Datos de control
			string ciudadRes, deptoRes, partido, paraPresidencia, key;

			// A reading order must be predefined and data must be separated by spaces. For every field in text, one field must be parsed.
			// Input must have _ in case a word accepts spaces.
			getline(file, key, delim);
			getline(file, candd.nombre, delim);
			getline(file, candd.apellido, delim);
			getline(file, candd.id, delim);
			getline(file, candd.genero, delim);
			getline(file, candd.estadoCivil, delim);
			getline(file, candd.fechaNacimiento, delim);
			getline(file, candd.ciudadNac, delim);
			getline(file, candd.ciudadRes, delim);
			getline(file, candd.deptoResidencia, delim);
			getline(file, candd.partido, delim);
			getline(file, candd.aPresidencia, '\n');

			istringstream(key) >> candd.key;


			// Meter el depto del candidato a simulacion o ya esta?
			Departamento * dep = cntrDptos.find(candd.deptoResidencia);
			if (!dep) {
				dep = new Departamento(candd.deptoResidencia);
				cntrDptos.insert(*dep);
			}
			Ciudad* ciud = dep->ciudades->find(candd.ciudadRes);
			if (!ciud) {
				ciud = new Ciudad(candd.ciudadRes);
				dep->ciudades->insert(*ciud);
			}
			Partido* partd = dep->partidos->find(candd.partido);
			if (!partd) {
				partd = new Partido(candd.partido);
				dep->partidos->insert(*partd);
			}

			// Ramificacion: Alcalde o Presidente?
			// Consume la linea siguiente a la de un Presidente como la de su vicepresidente
			if (candd.aPresidencia == "0") {
				//Entra candidato a alcaldia
				//Se crea un nodo de SSLL con sus datos
				//Se obtiene un apuntador a la estructura candidato en la lista global de candidatos
				Candidato* canddd = alcaldes.insert(candd);
				//Se añande dicho apuntados a las listas de Ciudad y Partido de tipo AVL<Candidato*>
				ciud->candidatos->insert(canddd);
				partd->candidatos->insert(canddd);
			}
			else {
				getline(file, key, delim);
				getline(file, vice.nombre, delim);
				getline(file, vice.apellido, delim);
				getline(file, vice.id, delim);
				getline(file, vice.genero, delim);
				getline(file, vice.estadoCivil, delim);
				getline(file, vice.fechaNacimiento, delim);
				getline(file, vice.ciudadNac, delim);
				getline(file, vice.ciudadRes, delim);
				getline(file, vice.deptoResidencia, delim);
				getline(file, vice.partido, delim);
				getline(file, vice.aPresidencia, '\n');

				istringstream(key) >> vice.key;

				canddPres.presidente = candd;
				canddPres.vicePresidente = vice;

				candidaturasPresidenciales.insert(canddPres);
				//cout << "-Presidente added";
			}
		}

		return true;

	}

	string reporteAlcaldes() {
		LinkedList<Candidato>* it = alcaldes.asList();
		ostringstream ss;
		ss << '\n';
		ss << std::left << setw(32) << "Nombre Completo" << setw(16) << "ID" << setw(5) << "Edad\t" << setw(2) << "Genero\t" << setw(16) << "Ciudad Res." << setw(10) << "Partido" << '\n'; 				//Partido no debe ser imprimido pero... pues
		ss << "--------------------------------------------------------------------------------------\n";
		for (int i = 1; i <= (*it).size(); i++) {
			//ss << i << " " << describirCandidato((*it).get(i));
			ss << describirCandidato((*it).get(i));
		}
		return ss.str();
	}

	string reportePresidentes() {
		LinkedList<CandidaturaPresidencial>* it = candidaturasPresidenciales.asList();
		ostringstream ss;
		ss << '\n';
		for (int i = 1; i <= (*it).size(); i++) {
			//ss << i << " " << "Prste:\t" << describirCandidato((*it).get(i).presidente)	<< "ViceP:\t" << describirCandidato((*it).get(i).vicePresidente);
			ss << "Prste:\t" << describirCandidato((*it).get(i).presidente) << "ViceP:\t" << describirCandidato((*it).get(i).vicePresidente);
		}
		return ss.str();
	}

	string reporteDeptos() {
		LinkedList<Departamento>* it = cntrDptos.asList();
		ostringstream ss;
		for (int i = 1; i <= (*it).size(); i++) {
			//ss << i << " " << (*it).get(i) << "\n";
			ss << (*it).get(i) << "\n";
		}
		return ss.str();
	}

	string describirCandidato(Candidato c) {
		ostringstream ss;
		ss << std::left
			<< setw(30) << c.nombre + " " + c.apellido << '\t'
			<< setw(12) << c.id << '\t'
			<< setw(5) << edad(c.fechaNacimiento) << '\t'					//Aparece en consultas
			<< setw(2) << c.genero << '\t'					//Aparece en consultas
			<< setw(10) << c.ciudadRes << '\t'			//Aparece en consultas excluy con Nacimiento
			<< setw(10) << c.partido << '\n'; 				//Partido no debe ser imprimido pero... pues
		return ss.str();
	}

	Departamento* getDpto(string nomb) {
		return cntrDptos.find(nomb);
	}

	//Departamento* getDpto(int key) {
	//	LinkedList<Departamento>* it = cntrDptos.asList();
	//	for (int i = 1; i <= (*it).size(); i++) {
	//		if (i == key) return &(*it).get(i);
	//	}
	//	return 0;
	//}


};