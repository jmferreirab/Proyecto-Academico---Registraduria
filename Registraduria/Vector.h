#include <stddef.h>

template <class T>
class Vector {

	T * p;
	int index;
	int size;

public:

	Vector(int size) {
		this->index = -1;
		this->size = size;
		this->p = new T[size];
	}

	~Vector() {
		delete[] p;
	}

	void push(T item) {		
		if (isFull()) return;
		++index;
		p[index] = item;
	}

	void pop_last() {
		if (isEmpty()) return;
		p[index] = -999999999;
		--index;

	}

	//Check last position of 0-index array. (index+1)==size
	bool isFull() {
		if (index+1 == size) return true;
		return false;
	}

	bool isEmpty() {
		if (index == -1) return true;
		return false;
	}

	// If called on an array with no elements, will return gibberish.
	T get(int pos) {
		if( size <= pos ) throw;
		return p[pos];
	}

	T* getVector() {
		return p;
	}

	int getSize() {
		return size;
	}

	//If 0 elements, then last returns 0 (at this point [0] is only gibberish;
	//Else, returns last element added. The index is always one ahead to be ready for addition.

	//New version
	int getLastPos() {							
		return index+1;						
	}
};
