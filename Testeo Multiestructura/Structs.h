#pragma once
#include <iostream>

using std::cout;

template <typename T>
class ItemCompuesto {
public:
	int dato;
	T* arb;
};

template <typename T>
class Nodo {

public:
	T dato;
	Nodo* izq, *der,*padre;
	
	template <typename T>
	Nodo(T datoN) {
		dato = datoN;
		izq = 0;
		der = 0;
		cout << "Katon haisekshu";
	}

	Nodo(const Nodo<T> &src) {
		if (src.raiz == 0) raiz = 0;
		else {
			copyTree(this->raiz, *&*src.raiz);
		}
		cout << "Called AVL CopyCTOR";
	}


};

template <typename T>
class Arbol {
public:

	Nodo<T> * raiz;
	Arbol() {
		raiz = 0;
	}

	//This swap is working. Effectively, contents from one tree are being exchanged with contents of the other
	//But if those contents were scoped and waiting for garbage collector then it will be for nil.
	Arbol<T>& operator=(Arbol<T> &srcRhs)
	{
		//if (this == &srcRhs)	return *this;

		//T tmp(srcRhs.raiz);
		//srcRhs.raiz = raiz;
		//raiz = tmp;

		//std::swap(*this, srcRhs);			
		//Bad idea to try to swap the container, overwrite the contents rather like in next line.
		cout << "Called AVL = OPERATOR\n";
		myswap(raiz, srcRhs.raiz);
		//std::swap(raiz, srcRhs.raiz);

		return *this;
	}

	template <typename T>
	void myswap(T lhs, T rhs) {
		T tmp(rhs);
		rhs = lhs;
		lhs = tmp;
	}

	//void myswap(Nodo<int> *lhs, Nodo<int>* rhs) {
	//	cout << "\nndo int\n";
	//	Nodo<int> *tmp(rhs);
	//	rhs = lhs;
	//	lhs = tmp;
	//}

	Arbol(const Arbol<T> *src) {
		if (src.raiz == 0) raiz = 0;
		else {
			copyTree(this->raiz, *&*src.raiz);
		}
		cout << "Called AVL CopyCTOR";
	}

	Arbol<T>* insertar(ItemCompuesto<T> *datoAInsertar) {
		return this;
	}

	Arbol<int>* insertar(int datoAInsertar) {
		
		Nodo<int>* cursorNode = raiz, *parent;
		
		if (!raiz) {
			cursorNode = new Nodo<int>(datoAInsertar);
			raiz = &*cursorNode;
			return this;
		}

		while (cursorNode != 0) {
			parent = cursorNode;
			if (datoAInsertar < cursorNode->dato) {				
				cursorNode = cursorNode->izq;					
				if (!cursorNode) {
					cursorNode = new Nodo<int>(datoAInsertar);
					parent->izq = cursorNode;
					return this;
				}
			}
			else cursorNode = cursorNode->der;	
		}		
		


		cout << raiz << "\t" << cursorNode << "\n";
		//cout << (*cursorNode).dato << "\n";
		//cout << (*raiz).dato << "\t" << (*cursorNode).dato << "\n";
		return this;
	}

private:



	void copyTree(Nodo<T> *&thisRoot, Nodo<T> *&sourceRoot)
	{
		if (sourceRoot == NULL)
		{
			thisRoot = NULL;
		}
		else
		{
			thisRoot = new Nodo<T>(sourceRoot->data);
			copyTree(thisRoot->izq, sourceRoot->izq);
			copyTree(thisRoot->der, sourceRoot->der);
		}
	}

};


