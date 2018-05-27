#include <iostream>
#include "Structs.h"
#include <cstdlib>

using std::cout;

template <typename T>
void print(T dato) {
	cout << '\n' << dato;
}

template <typename T>
T next(T &dato) {
	if (dato->izq == 0) return NULL;
	dato = dato->izq;
	return dato;
}

void impresionTipo1(Nodo<int> ** nodo) {
	Nodo<int> *nodo1 = *nodo;
	nodo1->dato = 555;
	print(nodo1->dato);
	print(nodo1->izq->dato);
	print(nodo1->izq->izq->dato);	
	print(nodo1->izq->izq->izq->dato);
	print(nodo1->izq->izq->izq->izq->dato);
}
void impresionTipo2(Nodo<int> * nodo) {
	nodo->dato = 555;
	print(nodo->dato);
	print(nodo->izq->dato);	
	print(nodo->izq->izq->dato);
	print(nodo->izq->izq->izq->dato);
	print(nodo->izq->izq->izq->izq->dato);
}

/*
	Comportamiento esperado hasta que se trata de meter en item el arbol creado en la funcion.
	Dicho arbol muere porque esta atado al contexto temporal de la funcion meter.
	For this, function ItemCompuesto was int dato, T arb.
*/
void meter(int dato, ItemCompuesto<Arbol<int>> item) {
	//Arbol<int> tempTree;
	//for (; dato > 0; dato--) tempTree.insertar(dato);

	//Nodo<int> * nodo = tempTree.raiz;
	//while (nodo != 0) {
	//	print(nodo->dato);
	//	nodo = next(nodo);
	//}

	//nodo = tempTree.raiz;

	//{
	//	//Both achieve the same modification directly on the tree. No clones.
	//	impresionTipo1(&nodo);
	//	impresionTipo2(nodo);
	//	//print(tempTree.raiz->dato);
	//	//print(tempTree.raiz->izq->dato);
	//	//print(tempTree.raiz->izq->izq->dato);
	//	//print(tempTree.raiz->izq->izq->izq->dato);
	//	//print(tempTree.raiz->izq->izq->izq->izq->dato);
	//}
	////item.arb = Arbol<int>(tempTree);
	//item.arb = tempTree;
}


/*
	Para desconectar el arbol creado en la funcion del scope de esta se pasa a separar
	memoria con new, y por tanto a hacerlo un puntero. Dada la "complejidad" del 
	tipo ItemCompuesto, entendiendo por esto el tener un dato de tipo T* arbol,
	se debe pasar ItemCompuesto a la funcion como un puntero. De este modo garantizamos 
	el enlazado del arbol creado en la funcion sin volver a copiarlo.

	Esto deja la necesidad de borrar cuando se invoque el destructor de ItemCompuesto al
	arbol usando delete T;
*/
void meterFixed(int dato, ItemCompuesto<Arbol<int>>* item) {
	Arbol<int> *tempTree = new Arbol<int>();
	for (; dato > 0; dato--) tempTree->insertar(dato);

	Nodo<int> * nodo = tempTree->raiz;
	while (nodo != 0) {
		print(nodo->dato);
		nodo = next(nodo);
	}

	nodo = tempTree->raiz;

	{
		//Both achieve the same modification directly on the tree. No clones.
		impresionTipo1(&nodo);
		impresionTipo2(nodo);
		//print(tempTree->raiz->dato);
		//print(tempTree->raiz->izq->dato);
		//print(tempTree->raiz->izq->izq->dato);
		//print(tempTree->raiz->izq->izq->izq->dato);
		//print(tempTree->raiz->izq->izq->izq->izq->dato);
	}
	item->arb = tempTree;
}

int main() {
	ItemCompuesto<Arbol<int>> i1;
	meterFixed(5, &i1);

	Nodo<int>* nodo = i1.arb->raiz;
	std::cout << nodo->dato;
}
