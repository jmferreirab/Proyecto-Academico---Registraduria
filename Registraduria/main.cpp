#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Herramientas.h"
#include <algorithm>
#include <sstream>
#include <stdlib.h>
#include <math.h>
#include <cassert>
#include "AVL.h"
#include "IndexedList.h"
#include "StructLib.h"

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

std::string describirCanddt(Candidato c) {
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

std::string replace(std::string line, const std::string& substr,const std::string& replace_with)
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
/// Para hacerlo tiene que examinar cada cadena y luego reescribirla. Requiere la funcion replace
void editarArchivoV2(const std::string &toReplace, const std::string &replacement, const std::string filepath)
{	
	// get a temporary file name
	char tmp_file_name[L_tmpnam];
	std::tmpnam(tmp_file_name);

	// create a 2 files. One temporary file with replaced text. Replace algorithm will looks at every string in the file.
	{
		std::ifstream original_file(filepath.c_str());
		std::ofstream temp_file(tmp_file_name);
		std::string line;
		while (std::getline(original_file, line)) {
			temp_file << replace(line, toReplace, replacement);
			if (original_file.peek() != EOF) temp_file << '\n';
		}
	}
	// overwrite the original file with the temporary file
	{
		std::ifstream temp_file(tmp_file_name);
		std::ofstream original_file(filepath.c_str());
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
	// using algorithm resembling editarArchivoV2
}

//Version 2. Su parametro paso a tipo Single Sorted Linked List. 
//Esta funcion empaca los contenidos del archivo siguiendo reglas fijas sobre el formato esperado en el archivo.
//El parametro es SSLL dado que este tipo de lista es capaz de sortear sobre la clave de tipo string al momento de insertar.
//Tras ejecutar la funcion, list (dependiendo de la clave que la funcion) elija, estara sorteada por clave.
//A medida que nuevos candidatos aparecen eston son empacan en la jerarquia compuesta data por AVLTree<Departamento>
bool obtenerDatosArchivo(SSLL<string, Candidato> &list, SSLL<string, CandidaturaPresidencial> &lisPres, AVLTree<Departamento> &deptos) {
	//std::ifstream file("C:\\Data\\Data.txt");
	std::ifstream file("Data.txt");

	Data<string, Candidato> nodoAlcaldia;
	Data<string, CandidaturaPresidencial> nodoPres;
	Candidato candd;
	CandidaturaPresidencial canddPres;
	Candidato vice;

	char delim = '+';


	if (!file)
	{
		cerr << "No se pudo leer el archivo de entrada\n";
		//cin.get();
		exit(0);
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
	while (list.next(dd) != NULL) {
		dd = list.next(dd);
		cout << std::left << setw(10) << edad(dd->data.info.fechaNacimiento).substr(0,3) << '\n'; 				//Partido no debe ser imprimido pero... pues
	}
}
void imprimirCandidatos2(SSLL<string, Candidato> &list) {
	SSLL<string,Candidato>::Iterator it = list.itBegin();
	++it;
	while (it != list.itEnd()) {		
		cout << std::left << setw(10) << edad((*it).info.fechaNacimiento).substr(0, 3) << '\t';		//No aparece
		++it;
	}
}

void imprimirAlcaldes(SSLL<string, Candidato> &list) {
	SSLL<string, Candidato>::Iterator it = list.itBegin();
	cout << std::left << setw(32) << "Nombre Completo" << setw(16) << "ID" << setw(5) << "Edad\t" << setw(2) << "Genero\t" << setw(16) << "Ciudad Res." << setw(10) << "Partido" << '\n'; 				//Partido no debe ser imprimido pero... pues
	cout << "--------------------------------------------------------------------------------------\n";
	while (it.hasNext()) {
		const Data<string, Candidato> *data = it.next();	//*data shares the same address of currentNode->data
		cout << describirCanddt(data->info);
	}
}

void imprimirPresidentes(SSLL<string, CandidaturaPresidencial> &list) {
	SSLL<string, CandidaturaPresidencial>::Iterator it = list.itBegin();
	std::cout << '\n';
	while (it.hasNext()) {
		const Data<string, CandidaturaPresidencial> *data = it.next();	//*data shares the same address of currentNode->data
		cout << "Prste:\t"	<<	describirCanddt(data->info.presidente)
		<< "ViceP:\t" << describirCanddt(data->info.vicePresidente);
	}
}


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
	assert(a < b);
	assert(c < d);
}

void runUnitTests() {
	unitTestDepartamento();
}


//En vez de tratar de templetear cada traversal (Nivel ciudad, los candidatos), (Nivel partido, los candidatos), (Nivel Depto, las ciudades), (Nivel Global, los dptos)
//Meter un parametro int y hacer un switch case en cada traversal que permita seleccionar una ruta con una funcion, como borrar o modificar.

void traverseCandidatos_City(AVLNode<Candidato*>* root) {
	AVLNode<Candidato*>* cdd = root;
	Pila<AVLNode<Candidato*>*> stackCdd;
	AVLNode<Candidato*>* lastNodeC = NULL;
	AVLNode<Candidato*>* peekNodeC = NULL;
	while (!stackCdd.PilaVacia() || cdd) {
		if (cdd) {
			stackCdd.push(cdd);
			cdd = cdd->left;
		}
		else {
			cdd = stackCdd.pop();
			cout << cdd->data->nombre << "\n";
			cdd = cdd->right;
		}
	}
}


void traverseCiudad_Depto(AVLNode<Ciudad>* root) {
	AVLNode<Ciudad>* cNode = root;
	Pila<AVLNode<Ciudad>*> stackC;
	AVLNode<Ciudad>* lastNodeC = NULL;
	AVLNode<Ciudad>* peekNodeC = NULL;
	while (!stackC.PilaVacia() || cNode) {
		if (cNode) {
			stackC.push(cNode);
			cNode = cNode->left;
		}
		else {
			cNode = stackC.pop();
			traverseCandidatos_City(cNode->data.candidatos->root);
			//f(cNode->data.candidatos->root);
			cNode = cNode->right;
		}
	}
}

template <typename Func>
void traverseDepts_Global(AVLNode<Departamento>* root, Func f) {
	AVLNode<Departamento>* node = root;
	Pila<AVLNode<Departamento>*> stack;
	AVLNode<Departamento>* lastNode = NULL;
	AVLNode<Departamento>* peekNode = NULL;
	while (!stack.PilaVacia() || node) {
		if (node) {
			stack.push(node);
			node = node->left;
		}
		else {
			node = stack.pop();
			//traverseCiudad_Depto(node->data.ciudades->root);
			f(node->data.ciudades->root);
			node = node->right;
		}
	}
}

int main() {

	editarArchivoV2("Yen+ +", "Jennifer+Novoa+", "Data.txt");
	//editarArchivoV2("Jose Manuel Ferreira Benavides", "Naga", "Data.txt");

	//Proveer una lista gestionada por Colombia (maximo ente)=AVLTree<Departamento> cntrDptos con datos de todos los deptos en simulacion.
	// Como saber a que departamento corresponde un candidato ? 
	// Si el candidato solo tiene ciudad haria falta un mapa o arreglo que relacione con alguna regla a cada ciudad con cada departamento,
	// de lo contrario este proceso es imposible.
	// Se decide entonces que el programa no deducira el departamento sino que lo obtendra de los datos del candidato.
	// Bajo esta consideración, en la medida que se lean los candidatos se revisara si el depto de ese candidato esta enlistado y si no lo esta se añadira a lista de dptos.

	SSLL<string, Candidato> alcaldes;
	SSLL<string, CandidaturaPresidencial> candidaturasPresidenciales;
	AVLTree<Departamento> cntrDptos;

	obtenerDatosArchivo(alcaldes, candidaturasPresidenciales, cntrDptos);
	imprimirAlcaldes(alcaldes);
	imprimirPresidentes(candidaturasPresidenciales);
	
	string dpt1 = "Bolivar", dpt2 = "Meta", pt1 = "Partido Rojo";
	cout << "Inorder AVL Departamentos:\n" << cntrDptos.traverseInOrder() << '\n';
	cout << "Ciudades AVL en Inorder del AVL del Dpto " << dpt1 << ":=\n" << (cntrDptos.find(dpt1))->ciudades->traverseInOrder() << '\n';
	cout << "Ciudades AVL en Inorder del AVL del Dpto " << dpt2 << ":=\n" << (cntrDptos.find(dpt2))->ciudades->traverseInOrder() << '\n';
	cout << "Partidos AVL para el Dpto " << dpt1 << ":=\n" << (cntrDptos.find(dpt1))->partidos->traverseInOrder() << '\n';
	cout << "Candidatos Partido Rojo en el dpto " << dpt2 << ":=\n" << (cntrDptos.find(dpt2))->partidos->find(pt1)->candidatos->traverseInOrderPointer() << '\n';

	runUnitTests();

	traverseDepts_Global(cntrDptos.root, traverseCiudad_Depto);

	//vector<string> candd = { "Paul", "Paul", "Paul" };
	//ResultadosCiudadX.pos5;

	//pos1 Candd en ciudad <--------> ResultadosCiudadX.pos5;

	//string res = "Nombre cad: " + candd.pos1 + "Votos por " + ResultadosCiudadX.pos5;

	//	for (size_t i = 0; i < candd.size; i++) {
	//		candd.at(i).votosPor = 500;
	//	}

	//menuModificarCandidato();

	//Herramientas_h::timeNow();  





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
