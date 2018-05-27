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


template<class Key, class DataT>
struct Data {
	Key key;
	DataT info;
};

template<class Key, class DataT>
struct IndexedNode {
	Data<Key, DataT> data;
	IndexedNode* next;
};
/*
	Template type Key defines the type used as index in structure "Data".
	This class 'SSLL' implementation is only guaranteed to work for when Key represents int types.
*/

template<class Key, class DataT>
class SSLL{
		
    int _size;
    
	IndexedNode<Key,DataT>* getNode(int index) {
	
		int pos = 1;
		IndexedNode<Key,DataT>* current = head;
	
		while (pos < index && current) {
			current = current->next;
			pos++;
		}
	
		if (pos == index) return current->next;
	
		return 0;
	}

public:     
	IndexedNode<Key, DataT>* head;		//Debe ser privado en lo posible
	class Iterator;

	SSLL(){
		_size = 0;
        head = new IndexedNode<Key,DataT>();
        head->next = NULL;
    }
    ~SSLL(){
        IndexedNode<Key,DataT>* tmp;
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

	DataT* insert(Data<Key, DataT> dataToAdd){		
	    IndexedNode<Key,DataT>* tmp = new IndexedNode<Key,DataT>();
		tmp->data = dataToAdd;
	    	   
        IndexedNode<Key,DataT>* current = head;  //holds the key for first node in a non-empty sll
        
        //find node before position to add at        
        while( current->next != NULL  &&  tmp->data.key > current->next->data.key   ){
            current = current->next;
        }
        
        tmp->next = current->next;  //Make new node link to whatever previous node linked to.
        current->next = tmp;        //Make previous node link to tmp newNode
        _size++;  

		return &(tmp->data.info);
	};	
	
	Data<Key,DataT> get(Key index) {
		IndexedNode<Key,DataT>* tmp = getNode(index);		
		return (tmp ? tmp->data : Data<Key,DataT>());
	}
	

	Data<Key,DataT> getByKey(Key keySrch){
		IndexedNode<Key,DataT>* x = head->next;
		while( x != NULL && x->data.key != keySrch){
			if( x->data.key > keySrch) return Data<Key,DataT>();
			x = x->next;
		}
		return x->data;		
	}
		
	void modify(DataT info, Key keySrch){
		
		IndexedNode<Key,DataT>* current = head->next;
				
		while( current != NULL && current->data.key != keySrch){
			if (current->data.key > keySrch ) return;
			//previous = current;
			current = current->next;			
		}		
		current->data.info = info;
	};
	
	void deleteKey(Key keySrch){
		IndexedNode<Key,DataT>* current = head;
		IndexedNode<Key,DataT>* target = NULL;
		
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

	IndexedNode<Key,DataT>* begin() {
		return head;
	}

	IndexedNode<Key, DataT>* next(IndexedNode<Key,DataT> *node) {
		//http://www.cplusplus.com/reference/iterator/next/
		//https://secweb.cs.odu.edu/~zeil/cs361/web/website/Lectures/iterators/page/iterators.html#implem
		//https://www.geeksforgeeks.org/iterators-c-stl/
		return node->next;
	}

	class Iterator {

	private:
		const IndexedNode<Key, DataT>* currentNode;
	public:

		//Constructor por defecto usa lista inicializadora para asignar currentNode a head
		Iterator() : currentNode (head) {	}

		//Constructor a partir de nodo. Usado por begin(), end()
		Iterator(const IndexedNode<Key, DataT>* nodo) : currentNode(nodo) {};

		///Cuando se iguale el operador a un nodo, se usara ese nodo como nodo actual. 
		///Como debe encapsularse, el retorno es una referencia a la instancia en uso de iterator
		Iterator& operator=(IndexedNode<Key, DataT>* nodo) {
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

		const Data<Key, DataT>* next() {
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

		Data<Key, DataT> operator*()
		{
			return currentNode->data;
		}

	};

};

#endif
