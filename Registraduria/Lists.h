#include <stddef.h>

#ifndef Lists_h
#define Lists_h
/*
	Manuel Ferreira Benavides. 2018.

	Removed cache and sorting functionality.
	Removed underscore notation for private members.
	Modified some name convention.
	Added implementation documentation.

*/

template<class T>
struct ListNode
{
	T data;
	ListNode<T> *next;
};

template <class T>
class LinkedList {

protected:
	
	int _size;
	ListNode<T> *root, *last;
	ListNode<T>* getNode(int index);

public:
	LinkedList();
	~LinkedList();
	

	/*
		Add(Insert) given object at the end of the list. Increase size.
	*/
	virtual bool insert(T);

	/*
	Add(Insert) given object at the start of the list. Increase size.
	*/
	virtual bool add_start(T);

	/*
	Add(Insert) given object at the given index of the list. Increase size.
	*/
	virtual bool add_at(T, int index);

	/*
	Set the object at index, with T;
	*/
	virtual bool set(T, int index);

	virtual int size();

	/*
	Remove first object;
	*/
	virtual T remove_first();

	/*
	Remove object at index;
	If index is not reachable, returns false;
	else, break link, free memory and decrement _size
	*/
	virtual T remove_pos(int index);
	
	/*
	Also known as Pop()
	*/
	virtual T remove_last();

	/*
	Get the index'th element on the list;
	Return Element if accessible,
	else, return false;
	*/
	virtual T get(int index);
};

template <class T>
LinkedList<T>::LinkedList() {
	root = NULL;
	last = NULL;
	_size = 0;
};

/*
	Until the head is null,
	Store the current head as a dummy pointer.
	Shift the actual head by one using it's next node.
	Free memory allocated to the previous head using the dummy pointer.
*/
template <class T>
LinkedList<T>::~LinkedList() {
	ListNode<T>* tmp;
	while (root != NULL)
	{
		tmp = root;
		root = root->next;
		delete tmp;
	}
	last = NULL;
};

/*
	Create a dummy node that will receive the passed data and will have no next element.
	If this node is the first node ever added, the make it both root and last.
	Else, 
		make the -to-be-updated last node point to the new to-be last node. (that is, the dummy)
		Then set the real last node to the dummy.
		Forget about the dummy.
*/

template<typename T>
int LinkedList<T>::size() {
	return _size;
}

template<typename T>
bool LinkedList<T>::insert(T t) {
	ListNode<T> *tmp = new ListNode<T>();
	tmp->data = t;
	tmp->next = NULL;

	if (root) {
		// Already have elements inserted
		last->next = tmp;
		last = tmp;
	}
	else {
		// First element being inserted
		root = tmp;
		last = tmp;
	}

	_size++;
	return true;
}

/*
	If attempting to add(insert) when list is empty, lets add_end() function solve the request.
	Otherwise, 
		create a dummy node and set its next field to the current root
		then, set this dummy to be the new current root linked to the previous root.
		Forget about the dummy.
*/

template<typename T>
bool LinkedList<T>::add_start(T t) {
	if (_size == 0) return insert(t);

	ListNode<T> *tmp = new ListNode<T>();
	tmp->next = root;
	tmp->data = t;
	root = tmp;

	_size++;

	return true;
}


/*
	Actual element inserted will live at index in the array ordering.
	The element index-1 will be forced to link to a dummy element we will add to the list.
	If auxiliar functions return a pointer this function evals to true, else to false.
*/
template<typename T>
bool LinkedList<T>::add_at(T t, int index) {

	if (index > _size)
		return insert(t);

	if (index == 1)  // 0
		return add_start(t);

	
	ListNode<T> *tmp = new ListNode<T>();
	ListNode<T> *prev = getNode(index-1);   // index-1
	tmp->data = t;
	tmp->next = prev->next;
	prev->next = tmp;

	_size++;
	return true;
}

/*
	Move along the sequence of links using the dummmy variable (current) until we're at the position requested
	Index-0 logic.
	For Index-1, calls must increment its argument by one.
*/

template<typename T>
ListNode<T>* LinkedList<T>::getNode(int index) {

	int pos = 1;
	ListNode<T>* current = root;

	while (pos < index && current) {
		current = current->next;
		pos++;
	}

	if (pos == index) return current;

	return 0;
}


template<typename T>
bool LinkedList<T>::set(T t, int index) {
	// Check if index position is in bounds
	if (index < 1 || index > _size)
		return false;

	getNode(index)->data = t;
	return true;
}

/*
	Returns the data at the removed node for validation
*/
template<typename T>
T LinkedList<T>::remove_last() {

	//nothing to remove or invalid call so return T() = 0;
	if (_size < 1)	return T();

	if (_size >= 2) {
		ListNode<T> *tmp = getNode(_size - 1);   // size -2
		
		//-2 because size is really an index. It is _size = desiredIndex + 1 so desiredIndex = _size - 1.
		//Furthermore, for deletion we operate using a double pointer from the previous to last element.

		T deleted_data = tmp->next->data;
		delete(tmp->next);
		tmp->next = NULL;
		last = tmp;
		_size--;
		return deleted_data;
	}
	else {
		// Only one element left on the list then both root and last must be null
		T deleted_data = root->data;
		delete(root);
		root = NULL;
		last = NULL;
		_size = 0;
		return deleted_data;
	}
}

/*
	Shift head by one. Similar to add_start but it must, aside from redefining the root, free the memory for the old root.
*/

template<typename T>
T LinkedList<T>::remove_first() {

	//nothing to remove or invalid call so return T() = 0;
	if (_size < 1)
		return T();

	if (_size > 1) {
		ListNode<T> *_next = root->next;
		T deleted_data = root->data;
		delete(root);
		root = _next;
		_size--;

		return deleted_data;
	}
	else {
		// Only one left, then let remove_last() solve it
		return remove_last();
	}

}

/*
	Get element at location index-1 and use its pointer to the next node to delete the node at index.
	Then shift the next of the item(index-1) to the next of the deleted elemnent to solve the links.
	If index asked if a special case (0 or size-1) (start or last) redirect the calls.
*/
template<typename T>
T LinkedList<T>::remove_pos(int index) {
	if (index < 1 || index > _size)
	{
		return T();
	}

	if (index == 1)
		return remove_first();

	if (index == _size)
	{
		return remove_last();
	}

	ListNode<T> *tmp = getNode(index-1);
	ListNode<T> *toDelete = tmp->next;		//not really needed variable
	T deletedData = toDelete->data;
	tmp->next = tmp->next->next;
	delete(toDelete);
	_size--;
	return deletedData;
}

template<typename T>
T LinkedList<T>::get(int index) {
	ListNode<T> *tmp = getNode(index);

	return (tmp ? tmp->data : T());
}

#endif
