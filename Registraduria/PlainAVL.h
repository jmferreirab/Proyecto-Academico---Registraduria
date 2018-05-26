#include <cmath>
#include <sstream>
#include <string>
#include "Pila.h"
#include "StructLib.h"

using std::stringstream;
using std::string;

template<class T>
struct AVLNode {

	AVLNode(const T & newData);
	~AVLNode();
	AVLNode<T> *left;
	AVLNode<T> *right;
	const T & data;
	int leftHeight;
	int rightHeight;
	int height();
};

template<class T>
class AVLTree {

	//Self balancing tree interface
public:

	AVLNode<T> * root;
	AVLTree();
	AVLTree(const T & element);
	~AVLTree();
	void insert(const T & element);
	bool remove(const T & element);
	const T * findMax();
	const T * find(const T & element);
	const std::string traverseInOrder(AVLNode<T>* node);

	void clear();

	/* Searches the AVLTree for the passed in element.
	* Returns NULL if the element cannot be found.
	*/
	template<> const Departamento * buscar<Departamento>(const Departamento & element) {
		return buscar(root, element);
	}

	/* Helper method for finding the passed in element in the subTree with subRoot as the root. */
	template<>	const Departamento * buscar(AVLNode<T> * subRoot, const T & element) {

		if (!subRoot)
			return NULL;
		else if (subRoot->data == element)
			return &subRoot->data;
		else if (subRoot->data > element)
			return find(subRoot->left, element);
		else
			return find(subRoot->right, element);
	}

private:

	//Helpers functions. Not the actual interface. Used to favor encapsulation
	
	// --------- Rotations

	//Balance handles the branches to choose when to do a single rotation and when to do a double rotation
	//Double rotations are defined within Balance as a combination of both of them of the SUBROOT and THE CHOOSEN CHILD

	AVLNode<T> * rotateRight(AVLNode<T> * subRoot) {
		AVLNode<T> * temp = subRoot->right;
		subRoot->right = temp->left;
		subRoot->rightHeight = temp->leftHeight;
		temp->left = subRoot;
		temp->leftHeight = subRoot->height() + 1;

		return temp;
	};

	AVLNode<T> * rotateLeft(AVLNode<T> * subRoot) {
		AVLNode<T> * temp = subRoot->left;
		subRoot->left = temp->right;
		subRoot->leftHeight = temp->rightHeight;
		temp->right = subRoot;
		temp->rightHeight = subRoot->height() + 1;

		return temp;
	};

	AVLNode<T> * balance(AVLNode<T> * subRoot) {

		AVLNode<T> * returnValue = subRoot;

		if (!subRoot) {

			returnValue = subRoot;
		}
		//If the left subTree is too large.
		else if (subRoot->leftHeight - subRoot->rightHeight > 2) {

			//Single rotation is necessary if the left-subtree is larger than the right.
			if (subRoot->left->leftHeight >= subRoot->left->rightHeight) {

				returnValue = rotateLeft(subRoot);
			}
			//Double rotation is necessary if the right-subtree is larger than the left.
			else {

				subRoot->left = rotateRight(subRoot->left);
				returnValue = rotateLeft(subRoot);
			}
		}
		//If the right subTree is too large.
		else if (subRoot->rightHeight - subRoot->leftHeight > 2) {

			//Single rotation is necessary if the right-subtree is larger than the left.
			if (subRoot->right->rightHeight >= subRoot->right->leftHeight) {

				returnValue = rotateRight(subRoot);
			}
			//Double rotation is necessary if the left sub-tree is larger than the right.
			else {

				subRoot->right = rotateLeft(subRoot->right);
				returnValue = rotateRight(subRoot);
			}
		}

		return returnValue;
	};

	void delete_traversal(AVLNode<T> * subRoot);
	AVLNode<T> * insert(AVLNode<T> * subRoot, const T & element);
	AVLNode<T> * remove(AVLNode<T> * subRoot, const T & element, bool * val);
	const T * find(AVLNode<T> * subRoot, const T & element);


	AVLNode<T> * findMax(AVLNode<T> * subRoot);

};

template<class T>
AVLTree<T>::AVLTree() { root = NULL; }

/* AVLTree constructor, sets root to element. */
template<class T>
AVLTree<T>::AVLTree(const T & element) {

	root = new AVLNode<T>(element);
}

/* Inserts the passed element into the AVLTree. */
template<class T>
void AVLTree<T>::insert(const T& element) {

	root = insert(root, element);
}

/* Helper method for inserting the passed element into the AVLTree. */
template<class T>
AVLNode<T> * AVLTree<T>::insert(AVLNode<T> * subRoot, const T & element) {

	AVLNode<T> * returnValue = subRoot;

	if (!subRoot) {

		returnValue = new AVLNode<T>(element);
	}
	else if (element >= subRoot->data) {

		subRoot->right = insert(subRoot->right, element);
		subRoot->rightHeight++;
	}
	else {

		subRoot->left = insert(subRoot->left, element);
		subRoot->leftHeight++;
	}

	return balance(returnValue);
}

/* Removes the passed element from the AVLTree (if it is present in the AVLTree). */
template<class T>
bool AVLTree<T>::remove(const T & element) {

	if (!root) {

		return false;
	}
	else {

		bool val = true;
		root = remove(root, element, &val);
		return val;
	}
}

/* Helper method to remove the passed element from the AVLTree (if it is present). */
template<class T>
AVLNode<T> * AVLTree<T>::remove(AVLNode<T> * subRoot, const T & element, bool * val) {

	AVLNode<T> * returnValue;

	if (!subRoot) {

		*val = false;
		returnValue = NULL;
	}
	else if (subRoot->data == element) {

		//No children of the found node, remove it.
		if (!subRoot->left && !subRoot->right) {

			delete subRoot;
			returnValue = NULL;
		}
		//Two children of the found node. Remove it, complexly.
		else if (subRoot->left && subRoot->right) {

			AVLNode<T> * maxLeft = findMax(subRoot->left);
			AVLNode<T> * newSubRoot = new AVLNode<T>(maxLeft->data);
			newSubRoot->left = remove(subRoot->left, maxLeft->data, val);
			newSubRoot->right = subRoot->right;
			newSubRoot->leftHeight = subRoot->leftHeight;
			newSubRoot->rightHeight = subRoot->rightHeight;
			//Subtract from leftHeight to account for removal call above.
			newSubRoot->leftHeight--;


			delete subRoot;
			returnValue = newSubRoot;
		}
		//One child of the found node. Remove it, and promote child.
		else {

			AVLNode<T> * temp = (subRoot->right) ? subRoot->right : subRoot->left;

			delete subRoot;
			returnValue = temp;
		}
	}
	else if (element >= subRoot->data) {

		subRoot->right = remove(subRoot->right, element, val);
		//If *val is true, then removal succeeded. Subtract from this node's rightHeight.
		if (*val)
			subRoot->rightHeight--;
		returnValue = subRoot;
	}
	else {

		subRoot->left = remove(subRoot->left, element, val);
		//If *val is true, then removal succeeded. Subtract from this node's leftHeight.
		if (*val)
			subRoot->leftHeight--;
		returnValue = subRoot;
	}

	return balance(returnValue);
}


/* Finds the maximum element in the AVLTree. */
template<class T>
const T * AVLTree<T>::findMax() {

	if (!root)
		return NULL;
	return findMax(root)->data;
}

/* Helper function for finding the maximum element in the tree. */
template<class T>
AVLNode<T> * AVLTree<T>::findMax(AVLNode<T> * subRoot) {

	if (subRoot->right)
		return findMax(subRoot->right);
	else
		return subRoot;
}

template<class T>
const std::string AVLTree<T>::traverseInOrder(AVLNode<T>* node)
{
	if (!node) return "";
	Pila<AVLNode<T>*> stack;
	std::ostringstream output;
	AVLNode<T>* lastNode = NULL;
	AVLNode<T>* peekNode = NULL;
	while (!stack.PilaVacia() || node) {
		if (node) {
			stack.push(node);
			node = node->left;
		}
		else {
			node = stack.pop();
			output << (node->data) << "\t";				//Store IT			
			//output << (node->left) << "\t";			//Store IT
			//output << (node->right) << "\t";			//Store IT			
			node = node->right;
		}
	}
	return output.str();
}

/* Empties out the AVLTree. */
template<class T>
void AVLTree<T>::clear() {

	delete_traversal(root);
	root = NULL;
}

/* AVLTree destructor, deletes all of the nodes left in the tree. */
template<class T>
AVLTree<T>::~AVLTree() {

	delete_traversal(root);
}


/* Deletes all the nodes in the subTree that has subRoot as the root. */
template<class T>
void AVLTree<T>::delete_traversal(AVLNode<T> * subRoot) {

	if (!subRoot)
		return;

	delete_traversal(subRoot->left);
	delete_traversal(subRoot->right);
	delete subRoot;
}


//~AVLNode functions---------------------------------------------------------------------
/* Constructor for AVLNode, sets the node's data to element. */
template<class T>
AVLNode<T>::AVLNode(const T & element) : data(element) {

	leftHeight = 0;
	rightHeight = 0;
	left = NULL;
	right = NULL;
}

/* Destructor for AVLNode. */
template<class T>
AVLNode<T>::~AVLNode() {

	left = NULL;
	right = NULL;
}

/* Gets the total height of the node. Adds together leftHeight & rightHeight. */
template<class T>
int AVLNode<T>::height() {

	return leftHeight + rightHeight;
}

/* Searches the AVLTree for the passed in element.
* Returns NULL if the element cannot be found.
*/
template<class T>
const T * AVLTree<T>::find(const T & element) {

	return find(root, element);
}

/* Helper method for finding the passed in element in the subTree with subRoot as the root. */
template<class T>
const T * AVLTree<T>::find(AVLNode<T> * subRoot, const T & element) {

	if (!subRoot)
		return NULL;
	else if (subRoot->data == element)
		return &subRoot->data;
	else if (subRoot->data > element)
		return find(subRoot->left, element);
	else
		return find(subRoot->right, element);
}