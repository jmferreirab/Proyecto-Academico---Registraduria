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
	IndexedNode<T, S>* head;
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
		return node->next;
	}

};

#endif
