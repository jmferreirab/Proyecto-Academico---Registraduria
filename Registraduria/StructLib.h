#pragma once
#include <iostream>

struct Candidato {

	string nombre, apellido, id;
	string genero;
	string estadoCivil;
	string fechaNacimiento, ciudadNac, ciudadRes, deptoResidencia;
	string partido;
	string aPresidencia;
};

struct CandidaturaPresidencial {
	Candidato presidente;
	Candidato vicePresidente;

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
	LinkedList<int> votosDepartamento;   //Every candidato is attached to a 'partido'

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
	Partido() {

	}
	string nombre;
	LinkedList<Candidato> *candidatos;
};

class Ciudad {
public:
	string nombre;
	Ciudad(string nom) : nombre(nom) {}

	inline bool operator==(const Ciudad &depto)const { return nombre == depto.nombre; }
	inline bool operator!=(const Ciudad &depto)const { return !(nombre == depto.nombre); }

	inline bool operator<(const Ciudad &depto)const { return nombre < depto.nombre; }
	inline bool operator>(const Ciudad &depto)const { return depto.nombre < nombre; }
	inline bool operator<=(const Ciudad &depto)const { return !(nombre > depto.nombre); }
	inline bool operator>=(const Ciudad &depto)const { return !(nombre < depto.nombre); }

	friend std::ostream& operator<<(std::ostream& out, const Ciudad& depto) { return out << depto.nombre; }

	Ciudad &operator=(Ciudad src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(*this, src); // now just swap the head of the copy 
							   // for the head of the source
		std::cout << "Called Ciudad =OP\n";
		return *this;
	}
	Ciudad(const Ciudad&d) {
		std::cout << "Called Departamento CopyCtor\n";
		nombre = d.nombre;
	}

};

class Departamento {
public:
	string nombre;
	AVLTree<Ciudad> ciudades;
	//LinkedList<Partido> partidos;

	Departamento(string nom) : nombre(nom) {}
	

	inline bool operator==(const Departamento &depto)const { return nombre == depto.nombre; }
	inline bool operator!=(const Departamento &depto)const { return !(nombre == depto.nombre); }

	inline bool operator<(const Departamento &depto)const { return nombre < depto.nombre; }
	inline bool operator>(const Departamento &depto)const { return depto.nombre < nombre; }
	inline bool operator<=(const Departamento &depto)const { return !(nombre > depto.nombre); }
	inline bool operator>=(const Departamento &depto)const { return !(nombre < depto.nombre); }

	friend std::ostream& operator<<(std::ostream& out, const Departamento& depto){ return out << depto.nombre;	}

	Departamento &operator=(Departamento src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(*this, src); // now just swap the head of the copy 
								   // for the head of the source
		std::cout << "Called Departamento =OP\n";
		return *this;
	}

	Departamento(const Departamento&d) {
		nombre = d.nombre;
		ciudades = d.ciudades;
		std::cout << "Called Departamento CopyCtor\n";
	}

};