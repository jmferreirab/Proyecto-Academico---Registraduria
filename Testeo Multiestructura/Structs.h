#pragma once
#include <iostream>
#include <typeinfo>

using std::cout;

template <typename T>
class Arbol;

template <typename T>
class ItemCompuesto {
public:
	int dato;
	Arbol<T>* arb;

	template <typename T>
	inline bool operator<(ItemCompuesto<T> &item)const { return dato < item.dato; }
	inline bool operator>(ItemCompuesto<T> &item)const { return item.dato < dato; }
	
	ItemCompuesto<T> &operator=(ItemCompuesto<T> src)
		// pass by reference performs the copy
	{
		//this->_size = src._size;
		std::swap(arb, src.arb); // now just swap the head of the copy 
							   // for the head of the source
		std::cout << "->ItemCompuesto<T> assignment op\n";
		return *this;
	}

};

template <typename T>
class Nodo {

public:
	T dato;
	Nodo* izq, *der,*padre;
	
	Nodo() {
		izq = 0;
		der = 0;
	}

	template <typename T>
	Nodo(T datoN) {
		dato = datoN;
		izq = 0;
		der = 0;
		cout << "->Node<T> ctr with dato:= " << dato << "\n";
	}

	template <>	Nodo(T* datoN) {
		//Needed even if bypassed by compiler
		dato = *datoN;
		izq = 0;
		der = 0;
		cout << "->Node<T+>\n";
	}

	//Nodo(const Nodo<T> &src) {
	//	if (src.raiz == 0) raiz = 0;
	//	else {
	//		copyTree(this->raiz, *&*src.raiz);
	//	}
	//	cout << "Called AVL CopyCTOR";
	//}


};

template <typename T>
class Arbol {
public:

	Nodo<T> * raiz;
	Arbol() {
		raiz = 0;
	}

	Arbol<T>* insertar(T *datoAInsertar) {
		cout << "Inserting into Tree<ItemCompuesto<int>> with first dato in ItemCompuesto own tree as " << datoAInsertar->arb->raiz->dato << "\n";
		Nodo<T>* cursorNode = raiz, *parent;		
		if (!raiz) {
			cursorNode = new Nodo<T>;
			cursorNode->dato = *datoAInsertar;
			raiz = &*cursorNode;
			
			return this;
		}
		while (cursorNode != 0) {
			parent = cursorNode;
			if (*datoAInsertar < cursorNode->dato) {
				cursorNode = cursorNode->izq;
				if (!cursorNode) {
					cursorNode = new Nodo<T>(datoAInsertar);
					parent->izq = cursorNode;
					return this;
				}
			}
			else cursorNode = cursorNode->der;
		}
		return 0;
	}

	Arbol<int>* insertar(int datoAInsertar) {
		cout << "->Override Insertar(int) to add " << datoAInsertar << " at " << this << "\n" ;

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

	//This swap is working. Effectively, contents from one tree are being exchanged with contents of the other
	//But if those contents were scoped and waiting for garbage collector then it will be for nil.
	//Copyng pointers doesn't call this tho. As expected.
	Arbol<T>& operator=(Arbol<T> &srcRhs)
	{
		cout << "Called AVL = OPERATOR\n";
		myswap(raiz, srcRhs.raiz);

		//if (this == &srcRhs)	return *this;

		//T tmp(srcRhs.raiz);
		//srcRhs.raiz = raiz;
		//raiz = tmp;

		//std::swap(*this, srcRhs);			
		//Bad idea to try to swap the container, overwrite the contents rather like in next line.
		//std::swap(raiz, srcRhs.raiz);

		return *this;
	}

	template <typename T>
	void myswap(T lhs, T rhs) {
		T tmp(rhs);
		rhs = lhs;
		lhs = tmp;
	}

	Arbol(const Arbol<T> *src) {
		if (src.raiz == 0) raiz = 0;
		else {
			copyTree(this->raiz, *&*src.raiz);
		}
		cout << "Called AVL CopyCTOR";
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


