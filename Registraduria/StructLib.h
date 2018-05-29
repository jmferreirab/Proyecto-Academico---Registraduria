#pragma once
#include <iostream>
#include <string>
#include "AVL.h"
#include "Herramientas.h"

using std::string;
using std::stringstream;

struct Candidato {

	int key;
	string nombre, apellido, id;
	string genero;
	string estadoCivil;
	string fechaNacimiento, ciudadNac, ciudadRes, deptoResidencia;
	string partido;
	string aPresidencia;

	inline bool operator==(const Candidato &candd)const { return key == candd.key; }
	inline bool operator!=(const Candidato &candd)const { return !(key == candd.key); }
	inline bool operator<(const Candidato &candd)const { return key < candd.key; }
	inline bool operator>(const Candidato &candd)const { return candd.key < key; }
	inline bool operator<=(const Candidato &candd)const { return !(key > candd.key); }
	inline bool operator>=(const Candidato &candd)const { return !(key < candd.key); }
	friend std::ostream& operator<<(std::ostream& out, const Candidato& c) {
		out << std::left
			<< std::setw(30) << c.nombre + " " + c.apellido << '\t'
			<< std::setw(5) << edad(c.fechaNacimiento) << '\t'					//Aparece en consultas
			<< std::setw(2) << c.genero << '\t'					//Aparece en consultas
			<< std::setw(10) << c.ciudadRes << '\t';			//Aparece en consultas excluy con Nacimiento
		return out;
	}

};

struct CandidaturaPresidencial {
	Candidato presidente;
	Candidato vicePresidente;

	inline bool operator==(const CandidaturaPresidencial &candd)const { return presidente.key == candd.presidente.key; }
	inline bool operator<(const CandidaturaPresidencial &candd)const { return presidente.key < candd.presidente.key; }
	inline bool operator>=(const CandidaturaPresidencial &candd)const { return !(presidente.key < candd.presidente.key); }
};


//ignore
struct Voto {
	const string candidatoAlcaldia;
	const string candidatoPresidencia;
	const string cedulaVotante;
	const string fechaNacimientoVotante;
	const string region;
};

class ResultadosPresidencia {
	//LinkedList<Voto> votosPresidencia;
	void stuff() {
		CandidaturaPresidencial p;
	}
};

class ResultadosCiudadX {
	//string departamento;
	//LinkedList<int> votosDepartamento;   //Every candidato is attached to a 'partido'

	//En votos departamento
	//pos 0 : #votos blanco
	//pos 1 : #votos nulos
	//pos 2 : #votos de hombres
	//pos 3 : #votos de mujers
	//pos 4 : #votos candidatoAlcaldia 1
	//pos 5 : #votos candidatoAlcaldia 2
	//pos 

};

class Partido {
public:
	string nombre;
	AVLTree<Candidato*> *candidatos;
	Partido() {}
	Partido(string nom) :nombre(nom) {
		candidatos = new AVLTree<Candidato*>();
	}


	inline bool operator==(const Partido &partd)const { return nombre == partd.nombre; }
	inline bool operator!=(const Partido &partd)const { return !(nombre == partd.nombre); }
	inline bool operator<(const Partido &partd)const { return nombre < partd.nombre; }
	inline bool operator>(const Partido &partd)const { return partd.nombre < nombre; }
	inline bool operator<=(const Partido &partd)const { return !(nombre > partd.nombre); }
	inline bool operator>=(const Partido &partd)const { return !(nombre < partd.nombre); }
	friend std::ostream& operator<<(std::ostream& out, const Partido& partd) { return out << partd.nombre; }

	Partido &operator=(Partido src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(nombre, src.nombre);
		std::swap(candidatos, src.candidatos);
		//std::cout << "Called Partido =OP\n";
		return *this;
	}
	Partido(const Partido&d) {
		//std::cout << "Called Departamento CopyCtor\n";
		nombre = d.nombre;
		candidatos = d.candidatos;
	}
};

class Ciudad {
public:
	string nombre;
	Ciudad() {}
	AVLTree<Candidato*> *candidatos;
	Ciudad(string nom) : nombre(nom) {
		candidatos = new AVLTree<Candidato*>();
	}

	inline bool operator==(const Ciudad &city)const { return nombre == city.nombre; }
	inline bool operator!=(const Ciudad &city)const { return !(nombre == city.nombre); }
	inline bool operator<(const Ciudad &city)const { return nombre < city.nombre; }
	inline bool operator>(const Ciudad &city)const { return city.nombre < nombre; }
	inline bool operator<=(const Ciudad &city)const { return !(nombre > city.nombre); }
	inline bool operator>=(const Ciudad &city)const { return !(nombre < city.nombre); }
	friend std::ostream& operator<<(std::ostream& out, const Ciudad& city) { return out << city.nombre; }

	Ciudad &operator=(Ciudad src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(nombre, src.nombre);
		std::swap(candidatos, src.candidatos);
		//std::cout << "Called Ciudad =OP\n";
		return *this;
	}
	Ciudad(const Ciudad&d) {
		//std::cout << "Called Departamento CopyCtor\n";
		nombre = d.nombre;
		candidatos = d.candidatos;
	}
};

class Departamento {
public:
	string nombre;
	AVLTree<Ciudad> *ciudades;
	AVLTree<Partido> *partidos;

	Departamento(string nom) : nombre(nom) {
		ciudades = new AVLTree<Ciudad>;
		partidos = new AVLTree<Partido>;
	}


	inline bool operator==(const Departamento &depto)const { return nombre == depto.nombre; }
	inline bool operator!=(const Departamento &depto)const { return !(nombre == depto.nombre); }
	inline bool operator<(const Departamento &depto)const { return nombre < depto.nombre; }
	inline bool operator>(const Departamento &depto)const { return depto.nombre < nombre; }
	inline bool operator<=(const Departamento &depto)const { return !(nombre > depto.nombre); }
	inline bool operator>=(const Departamento &depto)const { return !(nombre < depto.nombre); }
	friend std::ostream& operator<<(std::ostream& out, const Departamento& depto) { return out << depto.nombre; }

	Departamento &operator=(Departamento &src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(nombre, src.nombre); // now just swap the head of the copy 
		std::swap(ciudades, src.ciudades); // now just swap the head of the copy 
		std::swap(partidos, src.partidos);
		// for the head of the source
		//std::cout << "Called Departamento =OP\n";
		return *this;
	}

	Departamento() {};

	Departamento(const Departamento&d) {
		nombre = d.nombre;
		ciudades = d.ciudades;	//Calls assignemnet operator of AVL
		partidos = d.partidos;
		//std::cout << "Called Departamento CopyCtor\n";
	}

	Ciudad* getCiudad(string name) {
		return ciudades->find(name);
	}

	Partido* getPartido(string name) {
		return partidos->find(name);
	}

	//Ciudad getCiudad(int key) {
	//	LinkedList<Ciudad>* it = ciudades->asList();
	//	for (int i = 1; i <= (*it).size(); i++) {
	//		if (i == key) return (*it).get(i);
	//	}
	//}

	//Partido getPartido(int key) {
	//	LinkedList<Partido>* it = partidos->asList();
	//	for (int i = 1; i <= (*it).size(); i++) {
	//		if (i == key) return (*it).get(i);
	//	}
	//}

	//Getters are broken due to LL

};