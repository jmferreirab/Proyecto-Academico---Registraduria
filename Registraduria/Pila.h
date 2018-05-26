#pragma once

#include <stdlib.h>

#ifndef PILA_H     
#define PILA_H  

template <typename T>
struct Nodo {
	T dato;
	Nodo *sig;
};

/*
A dynamic size collection implementing operations of

push -> insert new Node at the top (sentinel.next = newNode)
pop -> removes the element at the top of the stack (delete sentinel.next)
*/

template <typename T>
class Pila {
	Nodo<T> *cab;

public: 
	
	Pila() {
		cab = new Nodo<T>;
		cab->dato = NULL;
		cab->sig = NULL;
	}

	void push(T v) {
		Nodo<T> *t = new Nodo<T>;
		t->dato = v;
		t->sig = cab->sig;
		cab->sig = t;
	};

	T pop() {
		T x;
		Nodo<T> *t = cab->sig;
		cab->sig = t->sig;
		x = t->dato;
		delete t;
		return x;
	}

	T peek() {
		return cab->sig->dato;
	}

	bool PilaVacia() {
		return cab->sig == NULL;
	}

	~Pila() {
		Nodo<T> *t = cab;
		while (t != NULL) {
			cab = t;
			t = t->sig;
			delete cab;
		}
	}
};



#endif
