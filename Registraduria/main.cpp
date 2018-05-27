#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Lists.h"
#include <iomanip>
#include "Herramientas.h"
#include <algorithm>
#include <sstream>
#include <random>
#include <math.h>
#include <cassert>

#include<windows.h>
#include "AVL.h"
#include "IndexedList.h"
#include "StructLib.h"
#include <io.h>
#include <fcntl.h>

//----Consejo general para implementer funciones que manipulen listas o colecciones. https://stackoverflow.com/questions/10476665/c-avoiding-copy-with-the-return-statement
//----Evidencia de soporte en cadenas para operadores < > == https://www.geeksforgeeks.org/comparing-two-strings-cpp/
//----Fuente del listado de municipios: https://es.wikipedia.org/wiki/Anexo:Municipios_de_Colombia_por_poblaci%C3%B3n

using namespace std;

namespace ASCII {
	unsigned char a = 201;
	unsigned char b = 205;
	unsigned char c = 187;
	unsigned char d = 186;
	unsigned char e = 200;
	unsigned char f = 188;
}

using namespace std;
using namespace ASCII;





std::string describir(Candidato c) {
	ostringstream ss;
	ss << std::left
		<< setw(30) << c.nombre + " " + c.apellido << '\t'
		<< setw(12) << c.id << '\t'
		<< setw(5) << edad(c.fechaNacimiento)  << '\t'					//Aparece en consultas
		<< setw(2) << c.genero << '\t'					//Aparece en consultas
		<< setw(10) << c.ciudadRes << '\t'			//Aparece en consultas excluy con Nacimiento
		<< setw(10) << c.partido << '\n'; 				//Partido no debe ser imprimido pero... pues
	return ss.str();
}



std::string replace(std::string line, const std::string& substr,
	const std::string& replace_with)
{
	std::string::size_type pos = 0;
	while ((pos = line.find(substr, pos)) != std::string::npos)
	{
		line.replace(pos, substr.size(), replace_with);
		pos += replace_with.size();
	}
	return line;
}

/// Crea 2 archivos. Uno temporal en el que toda ocurrencia de toReplace cambia a replacement en el archivo dado.
/// Para hacerlo tiene que examinar cada cadena y luego reescribirla.
void editarArchivoV2(const std::string &toReplace, const std::string &replacement, const std::string filepath)
{	
	// get a temporary file name
	char tmp_file_name[L_tmpnam_s];
	tmpnam_s(tmp_file_name, L_tmpnam_s);

	// create a 2 files. One temporary file with replaced text. Replace algorithm will looks at every string in the file.
	{
		std::ifstream original_file(filepath);
		std::ofstream temp_file(tmp_file_name);
		std::string line;
		while (std::getline(original_file, line))
			temp_file << replace(line, toReplace, replacement) << '\n';
	}
	// overwrite the original file with the temporary file
	{
		std::ifstream temp_file(tmp_file_name);
		std::ofstream original_file(filepath);
		original_file << temp_file.rdbuf();
	}
	// and delete the temporary file
	std::remove(tmp_file_name);
}

void editarArchivoV3() {
	//For Candidatos.txt
	//Loop over SSLL<string,candidato> lista... next!=null
	//At each step using an ofstream of("C:\\Data\\Data.txt")
	//do 
	// of << elementoActualEnlista.data.nombre <<  " \t" << elementoActualEnLista.data.fechaDeNacimiento ...
}

///Version 2. Su parametro paso a tipo Single Sorted Linked List. 
///Esta funcion empaca los contenidos del archivo siguiendo reglas fijas sobre el formato esperado en el archivo.
///El parametro es SSLL dado que este tipo de lista es capaz de sortear sobre la clave de tipo string al momento de insertar.
///Tras ejecutar la funcion list (dependiendo de la clave que la funcion) elija, estara sorteada por clave.
///Lo anterior es necesario para poder usar empacarCandidatosEnCiudades/1
bool obtenerDatosArchivo(SSLL<string, Candidato> &list, SSLL<string, CandidaturaPresidencial> &lisPres, AVLTree<Departamento> &deptos) {
	std::ifstream file("C:\\Data\\Data.txt");

	Data<string, Candidato> nodoAlcaldia;
	Data<string, CandidaturaPresidencial> nodoPres;
	Candidato candd;
	CandidaturaPresidencial canddPres;
	Candidato vice;

	char delim = '+';


	if (!file)
	{
		cerr << "No se pudo leer el archivo de entrada";
		return false;
	}

	//It would be a bad idea to directly create the Candidadto when reading from stream. What if we decide to delete the candidadte midway.
	while (!file.eof()) {

		//Datos de control
		string ciudadRes, deptoRes, partido, paraPresidencia;

		// A reading order must be predefined and data must be separated by spaces. For every field in text, one field must be parsed.
		// Input must have _ in case a word accepts spaces.
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
		
		// Meter el depto del candidato a simulacion o ya esta?
		Departamento * dep = deptos.find(candd.deptoResidencia);
		if (!dep) {
			dep = new Departamento(candd.deptoResidencia);			
			deptos.insert(*dep);
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
			nodoAlcaldia.info = candd;
			nodoAlcaldia.key = candd.ciudadRes;
			//Se obtiene un apuntador a la estructura candidato en la lista global de candidatos
			Candidato* canddd = list.insert(nodoAlcaldia);
			//Se añande dicho apuntados a las listas de Ciudad y Partido de tipo AVL<Candidato*>
			ciud->candidatos->insert(canddd);
			partd->candidatos->insert(canddd);

		}		
		else {
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

			canddPres.presidente = candd;
			canddPres.vicePresidente = vice;

			nodoPres.info = canddPres;
			nodoPres.key = canddPres.presidente.ciudadRes;
			lisPres.insert(nodoPres);

			//cout << "-Presidente";
		}
	}

	return true;

}

void imprimirCandidatos(SSLL<string, Candidato> &list) {
	IndexedNode<string, Candidato>* dd = list.begin();
	//cout << '\n';
	while (list.next(dd) != NULL) {
		dd = list.next(dd);
		cout << std::left
			<< setw(30) << dd->data.info.nombre << '\t'					//Aparece en consultas
			//<< setw(35) << dd->data.info.edad << '\t'					//Aparece en consultas
			//<< setw(35) << dd->data.info.sexo << '\t'					//Aparece en consultas
			//<< setw(35) << dd->data.info.ciudadNac << '\t'			//Aparece en consultas excluy con Residencia

			<< setw(12) << dd->data.info.id << '\t'						//No aparece
			<< setw(10) << edad(dd->data.info.fechaNacimiento).substr(0,3) << '\t'		//No aparece
			<< setw(20) << dd->data.info.ciudadRes << '\t'			//Aparece en consultas excluy con Nacimiento
			<< setw(30) << dd->data.info.partido << '\n'; 				//Partido no debe ser imprimido pero... pues
	}
}

void imprimirCandidatos2(SSLL<string, Candidato> &list) {
	SSLL<string,Candidato>::Iterator it = list.itBegin();
	++it;
	while (it != list.itEnd()) {		
		cout << std::left
		<< setw(30) << (*it).info.nombre << '\t'					//Aparece en consultas
		//<< setw(35) << dd->data.info.edad << '\t'					//Aparece en consultas
		//<< setw(35) << dd->data.info.sexo << '\t'					//Aparece en consultas
		//<< setw(35) << dd->data.info.ciudadNac << '\t'			//Aparece en consultas excluy con Residencia
		<< setw(12) << (*it).info.id << '\t'						//No aparece
		<< setw(10) << edad((*it).info.fechaNacimiento).substr(0,3) << '\t'		//No aparece
		<< setw(20) << (*it).info.ciudadRes << '\t'			//Aparece en consultas excluy con Nacimiento
		<< setw(30) << (*it).info.partido << '\n'; 				//Partido no debe ser imprimido pero... pues
		++it;
	}
}

void imprimirAlcaldes(SSLL<string, Candidato> &list) {
	SSLL<string, Candidato>::Iterator it = list.itBegin();
	std::cout << '\n';
	while (it.hasNext()) {
		const Data<string, Candidato> *data = it.next();	//*data shares the same address of currentNode->data
		cout << describir(data->info);
	}
}

void imprimirPresidentes(SSLL<string, CandidaturaPresidencial> &list) {
	SSLL<string, CandidaturaPresidencial>::Iterator it = list.itBegin();
	std::cout << '\n';
	while (it.hasNext()) {
		const Data<string, CandidaturaPresidencial> *data = it.next();	//*data shares the same address of currentNode->data
		cout << "Prste:\t"	<<	describir(data->info.presidente)
		<< "ViceP:\t" << describir(data->info.vicePresidente);
	}
}


LinkedList<Candidato>* obtenerListaCandidatosPorRegion(int codRegion) {
	return NULL;
};


///Llenar lista de candidatos
/// O bien usando lista indexada por codigo de ciudad
/// O bien sorteando la lista al terminar de insertar por codigo de ciudad
///El codigo de insersion podria modificarse para deducir la lista de ciudades en simulacion o simplemente tomarla las ciudades de archivo aparte.

///La lista sorteada presentara todos los candidatos a alcaldia. Los de una misma ciudad apareceran de forma consecutiva.
///En esta lista entonces se evidenciaran grupos de candidatos a una misma alcaldia.
///Entonces Con un solo condicional (siCiudadProcesadaActualmente!=CiudadNuevaEntrada) que aplicara sobre cada entrada de la lista
///Si ademas seguimos un estandar de orden alfabetico para las ciudades activas (con candidatos), se sabe que cada vez que el condicional falle se pasara a la siguiente ciudad en orden alfabetico
///Con lo anterior habran unificado los condicionales 
///if(nombreCiudad1) insertar candidato en ciudad1
///if(nombreCiudad2) insertar candidato en ciudad2
///...
///...
///if(noombreCiudad199) insertar candidato en ciudad199
///if(noombreCiudad200) insertar candidato en ciudad200

///En este contexto la ciudad tiene
///Arreglo CANDIDATOS de tamaño fijo=# partidos
///Arreglo RESULTADOS de tamaño votoNulo=0+votoBlanco=1+#partidos con candidato por ciudad
void empacarCandidatosEnCiudades(SSLL<string, Candidato> &list) {



}


//https://stackoverflow.com/questions/4700052/are-vector-a-special-case-of-linked-lists
//Revisar el enlace para definir que tipo de estructura de datos usar.   
//Revisar redundancia en usar Colombia.nombres y Deptos.Partidos etc...
//void consulta1(string depto, string partido) {
//	for (int i = 0; i < Colombia.nombresDptoosEnSimulacion.getSize(); i++) {
//		if (Colombia.nombresDptoosEnSimulacion.next().nombre == depto) {
//			for (int j = 0; j < Colombia.nombresPartidosEnSimulacion.getSize(); j++) {
//				if (Colombia.nombresPartidosEnSimulacion.next().nombre == partido) {
//					for (int k = 0; k < Deptos[i].Partidos[j].candidatos.getSize(); k++) {
//						cout << "Datos candidato " << Deptos[i].Partidos[j].candidatos.get(k);
//					}
//				}
//			}
//		}
//	}
//}



void codigoDescartado() {
	string dptos[] = {
		"Amazonas","Antioquia","Arauca","Atlantico","Bolivar","Boyaca",
		"Caldas","Caqueta","Casanare","Cauca","Cesar","Choco","Cordoba","Cundinamarca",
		"Guainia","Guaviare","Huila","La Guajira","Magdalena","Meta","Nariño",
		"Norte de Santander","Putumayo","Quindio","Risaralda","San Andres y Providencia",
		"Santander","Sucre","Tolima","Valle del Cauca","Vaupés","Vichada"
	};
}


void unitTestDepartamento() {
	Departamento a("Amazonia"), b("Huila");	
	Departamento c("Bolivar"), d("Cundinamarca");
	//assert(a < b);
	//assert(c < d);
}

void runUnitTests() {
	unitTestDepartamento();
}

int main() {

	//Proveer una lista gestionada por Colombia (maximo ente) con datos de todos los deptos en simulacion.
	// Como saber a que departamento corresponde un candidato ? 
	// Si el candidato solo tiene ciudad haria falta un mapa o arreglo que relacione con alguna regla a cada ciudad con cada departamento,
	// de lo contrario este proceso es imposible.
	// Se decide entonces que el programa no deducira el departamento sino que lo obtendra de los datos del candidato.
	// Bajo esta consideración, en la medida que se lean los candidatos se revisara si el depto de ese candidato esta enlistado y si no lo esta se añadira a lista de dptos.

	SSLL<string, Candidato> alcaldes;
	SSLL<string, CandidaturaPresidencial> candidaturasPresidenciales;
	AVLTree<Departamento> d;

	obtenerDatosArchivo(alcaldes, candidaturasPresidenciales, d);
	imprimirAlcaldes(alcaldes);
	imprimirPresidentes(candidaturasPresidenciales);


	string testDept = "Bolivar", testDept2 = "Meta", testPartido = "Rojo";
	cout << "\nInorder AVL Departamentos: " << d.traverseInOrder() << '\n';
	cout << "\nCiudades AVL en Inorder del AVL del Dpto " << testDept << "\n:= " << (d.find(testDept))->ciudades->traverseInOrder() << '\n';
	cout << "\nCiudades AVL en Inorder del AVL del Dpto " << testDept2 << "\n:= " << (d.find(testDept2))->ciudades->traverseInOrder() << '\n';
	cout << "\n\nPartidos AVL para el Dpto " << testDept << "\n:= " << (d.find(testDept))->partidos->traverseInOrder() << '\n';
	cout << "\n\nCandidatos en el arbol AVL del Partido Rojo en el dpto " << testDept2 << "\n:= " << (d.find(testDept2))->partidos->find("Partido Rojo")->candidatos->traverseInOrderPointer() << '\n';
	//runUnitTests();


	//menuModificarCandidato();

	//Herramientas_h::timeNow();  
	
	//cout << age("03/19/1997");

	LinkedList<Candidato> list;


	//File edition is adding an unwanted endline and causin clonation of a registry.--------------------------

	//editarArchivoV2("Ace", "Klasoxauria", "C:\\Data\\Data.txt");
	//editarArchivoV2("Jose Manuel Ferreira Benavides", "Naga", "C:\\Data\\Data.txt");


	//imprimirCandidatos(indexedList);

	


	//cout << indexedList.head->next->next->data.info.nombre;

//	obtenerDatosArchivo(list);


	////Contextoo CIUDAD
	//vector<string> ciudades = { "Paul", "Paul", "Paul" };

	//for (size_t i = 0; i < ciudades.size; i++)
	//{

	//vector<string> candd = { "Paul", "Paul", "Paul" };
	//ResultadosCiudadX.pos5;

	//pos1 Candd en ciudad <--------> ResultadosCiudadX.pos5;

	//string res = "Nombre cad: " + candd.pos1 + "Votos por " + ResultadosCiudadX.pos5;

	//	for (size_t i = 0; i < candd.size; i++) {
	//		candd.at(i).votosPor = 500;
	//	}

	//	
	//}



	//un candidato solo puede  ser lanzado por un psart politico de modo que cadapartido tiene un candidato por ciudad 

	//simularVotacion();
	//modificarCandidato();
	//generarReporteEstadistica(int tipoReporte);

}

void imprimirMenuPrincipal() {
	cout << "\t  BIENVENIDO AL MODULO DE ELECCIONES PRESIDENCIALES Y LOCALES" << endl;
	cout << endl;
	cout << "\t  Por favor, seleccione la accion que desea realizar:" << endl;
	cout << endl;
	cout << " 1. ADICIONAR UN REGISTRO" << endl;
	cout << endl;
	cout << " 2. ELIMINAR  UN REGISTRO" << endl;
	cout << endl;
	cout << " 3. MODIFICAR UN REGISTRO" << endl;
	cout << endl;
	cout << " 4. CONSULTAS" << endl;
	cout << endl;
	cout << " 5. REALIZAR SIMULACION " << endl;
	cout << endl;
	cout << " 7. SALIR" << endl;
	cout << endl;

	cout << "Opcion : ";

}

void menuModificarCandidato() {
	string id;
	cout << "\n----------SUBMENU DE MODIFICACION DE CANDIDATO---------\n\n";
	cout << "\nID del candidato: \t";
	cin >> id;
	//Perform search over candidadtos list
	//cout << "Elija un proceso: \n"
	//	<< "\n1. Modificación de nombre "
	//	<< "\n2. Modificación de ID "
	//	<< "\n3. Retiro de candidatura a alcaldia"
	//	<< "\n4. Retiro de candidatura a presidencia"
	//	<< "\n#. Volver al menu anterior.";


	int opcion = 0;
	char validar;
	int validacion = 0;
	cout << "\t \t" << a << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << c << endl;
	cout << "\t \t" << d << "  REGISTRADURIA NACIONAL DEL ESTADO CIVIL    " << d << endl;
	cout << "\t \t" << d << "                                             " << d << endl;
	cout << "\t \t" << d << " ELECCIONES 2018 (PRESIDENCIALES Y LOCALES)  " << d << endl;
	cout << "\t \t" << e << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << f << endl;
	cout << endl;
	do {


		imprimirMenuPrincipal();
		cin >> opcion;				//imprimirMenuPrincipal() pide al usuario la opcion
		switch (opcion) {
			//MODIFICACION REGISTROS -> 1 persona
		case 1: {
			break;
		}

		case 2: {

			break;
		}
		case 3: {

			break;
		}
				//CONSULTAS
		case 4: {

			int esp = 0;
			cout << "Seleccione la consulta que desea realizar" << endl;
			cout << "1. Mostrar lista de candidatos a la alcaldia por departamento y partido" << endl;
			cout << "2. Mostrar lista de candidatos a la alcaldia por partido de cada ciudad del     pais" << endl;
			cout << "3. Mostrar lista de candidatos a la presidencia y  vicepresidencia" << endl;
			cout << "4. Mostrar el nombre del candidato a la alcaldia por ciudad" << endl;
			cout << "5. Mostrar el tarjeton de candidatos a la alcaldia por ciudad" << endl;
			cout << "6. Mostrar el tarjeton de candidatos a la presidencia" << endl;
			cout << "7. Mostrar el censo electoral del pais" << endl;
			cout << "8. Edad de los candidatos" << endl;
			cout << "Opcion : "; cin >> esp;
			switch (esp) {
			case 1:
				cout << "Mostrando lista de candidatos" << endl;
				break;
			case 2:
				cout << "Mostrando lista de candidatos" << endl;
				break;
			case 3:
				cout << "Mostrando lista de candidatos" << endl;
				break;
			case 4:
				cout << "Mostrando el nombre del candidato" << endl;
				break;
			case 5:
				cout << "Mostrando el tarjeton" << endl;
				break;
			case 6:
				break;
			case 7:
				cout << "Mostrando el censo electoral..." << endl;
				break;
			case 8:
				cout << "Mostrando la edad de los candidatos" << endl;
				break;

			default:
				//aqui va la excepcion
				break;
			}

			break;// Fin de case==Consultas
		}

				//SIMULACION : GENERAR Y ASIGNAR NUMEROS ALEATORIOS CONSISTENTES CON LOS DATOS DEL CENSO ELECTORAL
		case 5: {
			cout << "Esta seguro de hacer la simulacion?, no podra hacer mas cambios s/n" << endl;
			cin >> validar;
			if (validar == 's') {
				cout << "Iniciando votaciones...." << endl;
				cout << "Mostrando escrutinios..." << endl;


				//GENERAR RESULTADOS///ESTADISTICAS  ->> DIFICIL -> 1 o 2 personas
				cin >> validar;
				switch (validar)
				{

				case 'a': {
					cout << "\nMostrar resultados por ciudad";
					break;
				}
				case 'b': {
					cout << "\nMostrar resultados presidencia";
					break;
				}

				}


			}


			break;
		}
		case 6: {

		}
		default:
			cout << " valor no permitido, debe ingresar un numero (1-5)" << endl;
			cin >> validacion;
		}
	} while (opcion != 6);


}
