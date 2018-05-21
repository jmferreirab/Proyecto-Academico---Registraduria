#include <stddef.h>
#include "ICollection.h"
#include <string>
#include <iterator>
#include <algorithm>

#ifndef IndexedList_h
#define IndexedList_h
//
//struct Data{
//	char* info;
//	IndexedNode *next;
//};
//
//struct IndexedNode{
//	int index;
//	Data d;
//};


template<class T, class S>
struct Data {
	T key;
	S info;
};

template<class T, class S>
struct IndexedNode {
	Data<T, S> data;
	IndexedNode* next;
};
/*
	Template type T defines the type used as index in structure "Data".
	This class 'SSLL' implementation is only guaranteed to work for when T represents int types.
*/

template<class T, class S>
class SSLL{
		
    int _size;
    
	IndexedNode<T,S>* getNode(int index) {
	
		int pos = 1;
		IndexedNode<T,S>* current = head;
	
		while (pos < index && current) {
			current = current->next;
			pos++;
		}
	
		if (pos == index) return current->next;
	
		return 0;
	}

public:     
	IndexedNode<T, S>* head;		//Debe ser privado en lo posible
	class Iterator;

	SSLL(){
		_size = 0;
        head = new IndexedNode<T,S>();
        head->next = NULL;
    }
    ~SSLL(){
        IndexedNode<T,S>* tmp;
		while (head != NULL)
		{
			tmp = head;
			head = head->next;
			delete tmp;
		}
	}

	// Raiz de la lista oculta en un iterador
	Iterator itBegin()
	{
		return Iterator(head);
	}

	// Cola de la lista oculta en un iterador
	Iterator itEnd()
	{
		return Iterator(0);
	}
  
    int size(){
    	return _size;
	}

	void insert(Data<T, S> dataToAdd){		
	    IndexedNode<T,S>* tmp = new IndexedNode<T,S>();
		tmp->data = dataToAdd;
	    	   
        IndexedNode<T,S>* current = head;  //holds the key for first node in a non-empty sll
        
        //find node before position to add at        
        while( current->next != NULL  &&  tmp->data.key > current->next->data.key   ){
            current = current->next;
        }
        
        tmp->next = current->next;  //Make new node link to whatever previous node linked to.
        current->next = tmp;        //Make previous node link to tmp newNode
        _size++;        
	};	
	
	Data<T,S> get(T index) {
		IndexedNode<T,S>* tmp = getNode(index);		
		return (tmp ? tmp->data : Data<T,S>());
	}
	

	Data<T,S> getByKey(T keySrch){
		IndexedNode<T,S>* x = head->next;
		while( x != NULL && x->data.key != keySrch){
			if( x->data.key > keySrch) return Data<T,S>();
			x = x->next;
		}
		return x->data;		
	}
		
	void modify(S info, T keySrch){
		
		IndexedNode<T,S>* current = head->next;
				
		while( current != NULL && current->data.key != keySrch){
			if (current->data.key > keySrch ) return;
			//previous = current;
			current = current->next;			
		}		
		current->data.info = info;
	};
	
	void deleteKey(T keySrch){
		IndexedNode<T,S>* current = head;
		IndexedNode<T,S>* target = NULL;
		
		while( current->next != NULL && current->next->data.key != keySrch){
			if (current->next->data.key > keySrch ) return;
			//previous = current;
			current = current->next;			
		}
		
		target = current->next; //to delete		
		current->next = current->next->next;
		
		delete target;
		
		_size--;
	}

	IndexedNode<T,S>* begin() {
		return head;
	}

	IndexedNode<T, S>* next(IndexedNode<T,S> *node) {
		//http://www.cplusplus.com/reference/iterator/next/
		//https://secweb.cs.odu.edu/~zeil/cs361/web/website/Lectures/iterators/page/iterators.html#implem
		//https://www.geeksforgeeks.org/iterators-c-stl/
		return node->next;
	}

	class Iterator {

	private:
		const IndexedNode<T, S>* currentNode;
	public:

		//Constructor por defecto usa lista inicializadora para asignar currentNode a head
		Iterator() : currentNode (head) {	}

		//Constructor a partir de nodo. Usado por begin(), end()
		Iterator(const IndexedNode<T, S>* nodo) : currentNode(nodo) {};

		///Cuando se iguale el operador a un nodo, se usara ese nodo como nodo actual. 
		///Como debe encapsularse, el retorno es una referencia a la instancia en uso de iterator
		Iterator& operator=(IndexedNode<T, S>* nodo) {
			this->currentNode = nodo;
			return *this;					
		}

		// Prefix ++ overload
		Iterator& operator++()
		{
			if (currentNode)
				currentNode = currentNode->next;
			return *this;
		}

		// Postfix ++ overload
		Iterator operator++(int)
		{
			Iterator iterator = *this;
			++*this;
			return iterator;
		}

		const Data<T, S>* next() {
			currentNode = currentNode->next;
			//cout << &(currentNode->data);
			return &(currentNode->data);
		}

		bool hasNext() {
			return !(currentNode->next == 0);
		}

		bool operator!=(const Iterator& iterator)
		{
			return currentNode != iterator.currentNode;
		}

		Data<T, S> operator*()
		{
			return currentNode->data;
		}

	};

};

#endif
