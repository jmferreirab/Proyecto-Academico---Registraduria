#include <iostream>
#include <fstream>
#include <string>
#include <iomanip>
#include "Registraduria.h"
#include <stdlib.h>
#include <math.h>
//#include <cassert>
#include "AVL.h"

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

using namespace ASCII;



std::string replace(std::string line, const std::string& substr, const std::string& replace_with)
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
	const char* tmpFile = "tempFile.txt";			// get a temporary file name

													// create a 2 files. One temporary file with replaced text. Replace algorithm will looks at every string in the file.
	{
		std::ifstream original_file(filepath.c_str());
		std::ofstream temp_file(tmpFile, std::ofstream::trunc);
		std::string line;
		while (std::getline(original_file, line)) {
			temp_file << replace(line, toReplace, replacement);
			if (original_file.peek() != EOF) temp_file << '\n';
		}
		//cout << temp_file.tellp() << '\n';	//tells how many character were passed to temp_file;

	}
	// overwrite the original file with the temporary file
	{
		std::ifstream temp_file(tmpFile);
		std::ofstream original_file(filepath.c_str(), std::ofstream::trunc);
		original_file << temp_file.rdbuf();
		//cout << temp_file.rdbuf();
	}

	std::remove(tmpFile);			// and delete the temporary file
}

void editarArchivoV3() {
	//For Candidatos.txt
	//Loop over SSLL<string,candidato> lista... next!=null
	//At each step using an ofstream of("C:\\Data\\Data.txt")
	//do 
	// of << elementoActualEnlista.data.nombre <<  " \t" << elementoActualEnLista.data.fechaDeNacimiento ...
	// using algorithm resembling editarArchivoV2
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
	//Departamento c("Bolivar"), d("Cundinamarca");
	//assert(a < b);
	//assert(c < d);
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

void localTest() {
	Registraduria r;
	r.obtenerDatosArchivo();
	cout << r.reporteAlcaldes();
	cout << r.reportePresidentes();

	string dpt1 = "Bolivar", dpt2 = "Meta", pt1 = "Partido Rojo";
	cout << "\nDepartamentos en simulacion:=\n" << r.reporteDeptos() << '\n';
	cout << "\nCiudades en simulacion de " << dpt1 << ":=\n" << r.getDpto(dpt1)->ciudades->traverseInOrder() << '\n';
	cout << "\nCiudades en simulacion de " << dpt2 << ":=\n" << r.getDpto(dpt2)->ciudades->traverseInOrder() << '\n';
	cout << "\nPartidos en simulacion del Dpto " << dpt1 << ":=\n" << r.getDpto(dpt1)->partidos->traverseInOrder() << '\n';
	cout << "Candidatos Partido Rojo en el dpto " << dpt2 << ":=\n" << r.getDpto(dpt2)->getPartido(pt1)->candidatos->traverseInOrderPointer() << '\n';

	runUnitTests();
}

void imprimirMenuPrincipal();
void opcionesPrincipal(Registraduria &r);

int main() {

	editarArchivoV2("Yen+ +", "Jennifer+Novoa+", "Data.txt");
	//editarArchivoV2("Jose Manuel Ferreira Benavides", "Naga", "Data.txt");

	//Proveer una lista gestionada por Colombia (maximo ente)=AVLTree<Departamento> cntrDptos con datos de todos los deptos en simulacion.
	// Como saber a que departamento corresponde un candidato ? 
	// Si el candidato solo tiene ciudad haria falta un mapa o arreglo que relacione con alguna regla a cada ciudad con cada departamento,
	// de lo contrario este proceso es imposible.
	// Se decide entonces que el programa no deducira el departamento sino que lo obtendra de los datos del candidato.
	// Bajo esta consideración, en la medida que se lean los candidatos se revisara si el depto de ese candidato esta enlistado y si no lo esta se añadira a lista de dptos.

	Registraduria r;
	r.obtenerDatosArchivo();

	opcionesPrincipal(r);

	//	traverseDepts_Global(cntrDptos.root, traverseCiudad_Depto);

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
	cout << "\t \t" << a << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << c << endl;
	cout << "\t \t" << d << "  REGISTRADURIA NACIONAL DEL ESTADO CIVIL    " << d << endl;
	cout << "\t \t" << d << "                                             " << d << endl;
	cout << "\t \t" << d << " ELECCIONES 2018 (PRESIDENCIALES Y LOCALES)  " << d << endl;
	cout << "\t \t" << d << "                                             " << d << endl;
	cout << "\t \t" << d << " Autores:                                    " << d << endl;
	cout << "\t \t" << d << " Jose Manuel Ferreira Benavides              " << d << endl;
	cout << "\t \t" << d << " Juliana Alexandra Capador                   " << d << endl;
	cout << "\t \t" << d << " Jorge Lucero                                " << d << endl;
	cout << "\t \t" << e << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << b << f << endl;

	cout << endl;

	cout << "\t  BIENVENIDO AL MODULO DE ELECCIONES PRESIDENCIALES Y LOCALES" << endl;
	cout << endl;
	cout << "\t  Por favor, seleccione la accion que desea realizar:" << endl;
	cout << endl;
	cout << "1. ADICIONAR UN REGISTRO" << endl;
	cout << endl;
	cout << "2. ELIMINAR  UN REGISTRO" << endl;
	cout << endl;
	cout << "3. MODIFICAR UN REGISTRO" << endl;
	cout << endl;
	cout << "4. CONSULTAS" << endl;
	cout << endl;
	cout << "5. REALIZAR SIMULACION " << endl;
	cout << endl;
	cout << "7. SALIR" << endl;
	cout << endl;

	cout << ">";

}

void opcionesPrincipal(Registraduria &r) {
	//string id;
	//cout << "\n----------SUBMENU DE MODIFICACION DE CANDIDATO---------\n\n";
	//cout << "\nID del candidato: \t";
	//cin >> id;
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
			system("cls");
			cout << "----CONSULTAS----\n\n";
			int esp = 0;
			cout << "Seleccione la consulta que desea realizar:" << endl;
			cout << "1. Mostrar lista de candidatos a la alcaldia por departamento y partido" << endl;
			cout << "2. Mostrar lista de candidatos a la alcaldia por partido de cada ciudad del     pais" << endl;
			cout << "3. Mostrar lista de candidatos a la presidencia y  vicepresidencia" << endl;
			cout << "4. Mostrar el nombre del candidato a la alcaldia por ciudad" << endl;
			cout << "5. Mostrar el tarjeton de candidatos a la alcaldia por ciudad" << endl;
			cout << "6. Mostrar el tarjeton de candidatos a la presidencia" << endl;
			cout << "7. Mostrar el censo electoral del pais" << endl;
			cout << "8. Edad de los candidatos" << endl;
			cout << "\n> "; cin >> esp;
			switch (esp) {
			case 1: {
				system("cls");
				string decision1 = "", decision2 = "";
				Departamento * dep;
				Partido *part;

				cout << "----CONSULTA DE CANDIDATOS A ALCALDIA POR DEPARTAMENTO Y PARTIDO----\n";
				cout << "\nSeleccione un departamento:\n\n";
				cout << r.reporteDeptos();
				cout << "\n>";
				cin.ignore();
				getline(cin, decision1, '\n');
				dep = r.getDpto(decision1);
				while (!dep) {
					cout << "Mismatch. Try again.\n";
					cin.clear();
					cout << ">";
					getline(cin, decision1, '\n');
					dep = r.getDpto(decision1);
				}
				cout << "\nSeleccione un partido:\n\n";
				cout << r.getDpto(decision1)->partidos->traverseInOrder();
				cout << "\n>";
				getline(cin, decision2, '\n');
				part = r.getDpto(decision1)->getPartido(decision2);
				while (!part) {
					cout << "Mismatch. Try again.\n";
					cin.clear();
					cout << ">";
					getline(cin, decision2, '\n');
					part = r.getDpto(decision1)->getPartido(decision2);
				}



				cout << "\n----------------------------------Candidatos Alcaldia---------------------------------\n\n";
				cout << part->candidatos->traverseInOrderPointer();
				cout << "\n--------------------------------------------------------------------------------------\n";
				cin.get();
				system("cls");
				break;
			}

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
			break;
		}
		default:
			if (opcion == 7) break;
			cout << " valor no permitido, debe ingresar un numero (1-5)" << endl;
			cin >> validacion;
		}
	} while (opcion != 7);


}