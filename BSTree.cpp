#include "BSTree.h"
#include <algorithm>

//Protected inner class BSTreeNode
template< typename DataType, typename KeyType> //Constructor for tree node
BSTree<DataType, KeyType>::BSTreeNode::BSTreeNode(const DataType& nodeDataItem, BSTreeNode* leftPtr, BSTreeNode* rightPtr) {
	dataItem = nodeDataItem;
	left = leftPtr;
	right = rightPtr;
}

//Public member functions

template< typename DataType, typename KeyType > //Default constructor for tree
BSTree<DataType, KeyType>::BSTree() { root = nullptr; }

template < typename DataType, typename KeyType > //Copy constructor for tree
BSTree<DataType, KeyType>::BSTree(const BSTree<DataType, KeyType>& other) {
	root = nullptr;
	*this = other;
}

template< typename DataType, typename KeyType > //Assignment operator overload
BSTree<DataType, KeyType>& BSTree<DataType, KeyType>::operator=(const BSTree<DataType, KeyType>& other) {
	clear();
	if (!other.isEmpty()) { copyHelper(other.root); }
	return *this;
}

template< typename DataType, typename KeyType > //Destructor
BSTree<DataType, KeyType>::~BSTree() {
	clear();
}

template< typename DataType, typename KeyType > //Insert a node
void BSTree<DataType, KeyType>::insert(const DataType& newDataItem) {
	if (isEmpty()) {
		try {
			root = new BSTreeNode(newDataItem, nullptr, nullptr);
		}
		catch (bad_alloc & e) {
			throw e;
		}
	}
	else { insertHelper(newDataItem, root); }
}

template< typename DataType, typename KeyType > //Retrieve a node
bool BSTree<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& searchDataItem) {
	if (isEmpty()) { return false; }
	else { return retrieveHelper(searchKey, searchDataItem, root); }
}

template< typename DataType, typename KeyType > //Remove a node
bool BSTree<DataType, KeyType>::remove(const KeyType& deleteKey) {
	bool keyFound = false;
	if (!isEmpty()) { keyFound = removeHelper(deleteKey, root, root); }
	return keyFound;
}

template < typename DataType, typename KeyType > //Write keys in ascending order
void BSTree<DataType, KeyType>::writeKeys() const {
	if (isEmpty()) { cout << "Tree is empty" << endl; }
	else { writeKeysHelper(root); }
	cout << endl;
}

template < typename DataType, typename KeyType > //Clear nodes from tree
void BSTree<DataType, KeyType>::clear() {
	if (!isEmpty()) { clearHelper(root); }
	root = nullptr;
}

template < typename DataType, typename KeyType > //Check if tree is empty
bool BSTree<DataType, KeyType>::isEmpty() const { return (root == nullptr); }

//Public in-lab operations

template < typename DataType, typename KeyType > //Return number of tree nodes
int BSTree<DataType, KeyType>::getCount() const {
	if (isEmpty())
		return 0;

	countHelper(root);
}

template < typename DataType, typename KeyType > //Return height of tree
int BSTree<DataType, KeyType>::getHeight() const { return heightHelper(root); }

//Private helper functions

/*
Helper for copy constructor.
This function does a prefix traversal of this tree, with the data processing
step being to insert the node from the tree "other" to this tree. The insert
method that was already defined handles insertion in O(log(n)) time, and this
order of insertion results in a tree that is identical to "other."
*/
template < typename DataType, typename KeyType >
void BSTree<DataType, KeyType>::copyHelper(const BSTreeNode * otherCurrent) {
	insert(otherCurrent->dataItem);
	if (otherCurrent->left != nullptr) { copyHelper(otherCurrent->left); }
	if (otherCurrent->right != nullptr) { copyHelper(otherCurrent->right); }
}

/*
Helper for insert function.
This function searches through the tree, going to either the left or right child depending upon
how the new data item's key compares to the key in the current node. In O(log(n)) time, it will
find where a new node must be inserted if it does not already exist. If there is a null pointer
to the location where the new node must be, it is placed there. If the node already exists, no
new memory needs allocation and the node there is just updated with the new data item. This
function also uses exception handling for errors in allocating new memory.
*/
template < typename DataType, typename KeyType >
void BSTree<DataType, KeyType>::insertHelper(const DataType & newDataItem, BSTreeNode * &current) {
	if (current == NULL) {
		try {
			current = new BSTreeNode(newDataItem, nullptr, nullptr);
		}
		catch (bad_alloc & e) {
			throw e;
		}
	}
	else if (newDataItem.getKey() < current->dataItem.getKey()) {
		insertHelper(newDataItem, current->left);
	}
	else if (newDataItem.getKey() > current->dataItem.getKey()) {
		insertHelper(newDataItem, current->right);
	}
	else { current->dataItem = newDataItem; }
}

/*
Helper for retrieve function.
This function searches through the tree like the insert function, determining whether the item to retrieve
exists or not by comparing keys in O(log(n)) time. This makes the function highly scalable to large tree sizes.
If the item to retrieve is found, searchDataItem takes on the data of that item. Because searchDataItem is passed
by reference, no unnecessary copies of searchDataItem are created.
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType, KeyType>::retrieveHelper(const KeyType & searchKey, DataType & searchDataItem, BSTreeNode * current) {
	bool keyFound = false;

	if (searchKey < current->dataItem.getKey() && current->left != nullptr) {
		keyFound = retrieveHelper(searchKey, searchDataItem, current->left);
	}
	else if (searchKey > current->dataItem.getKey() && current->right != nullptr) {
		keyFound = retrieveHelper(searchKey, searchDataItem, current->right);
	}
	else if (searchKey == current->dataItem.getKey()) {
		keyFound = true;
		searchDataItem = current->dataItem;
	}
	return keyFound;
}

/*
Helper for writeKeys function.
This function does an inorder traversal of the tree, printing out each node's data item
during the data processing step. This naturally results in the keys being listed in ascending order.
*/
template< typename DataType, typename KeyType >
void BSTree<DataType, KeyType>::writeKeysHelper(BSTreeNode * current) const {
	if (current->left != nullptr)
	{
		writeKeysHelper(current->left);
	}
	cout << current->dataItem.getKey() << " ";
	if (current->right != nullptr)
	{
		writeKeysHelper(current->right);
	}
}

/*
Helper for the clear function.
This function does a postorder traversal of the tree, freeing the allocated memory of each
node during the data processing step. This enables each node to be deleted in the proper
order to avoid memory leakage.
*/
template< typename DataType, typename KeyType >
void BSTree<DataType, KeyType>::clearHelper(BSTreeNode * current) {
	if (current->left != nullptr) { clearHelper(current->left); }
	if (current->right != nullptr) { clearHelper(current->right); }
	delete current;
}

/*
Helper for the remove function.
This function finds the node to remove (or determines it is not in the tree) in O(log(n)) time. For nodes
with less than 2 children, removal is simple, requiring adjustment of the parent node's pointer and deletion
of the current node. For nodes with 2 children, an extra step is required to keep the tree nodes in a proper structure.
We implemented a function called retrieveMinKey to quickly find the minimum key in the deletion node's right subtree.
This key is then put into the deletion node. The node that actually gets deleted is the minimum node in the right subtree,
since this will be a simple less-than-2-children deletion and it maintains a proper tree structure.
*/
template < typename DataType, typename KeyType >
bool BSTree<DataType, KeyType>::removeHelper(const KeyType & deleteKey, BSTreeNode * last, BSTreeNode * current) {
	bool keyFound = false;

	if (deleteKey < current->dataItem.getKey() && current->left != nullptr) {
		keyFound = removeHelper(deleteKey, current, current->left);
	}
	else if (deleteKey > current->dataItem.getKey() && current->right != nullptr) {
		keyFound = removeHelper(deleteKey, current, current->right);
	}
	else if (deleteKey == current->dataItem.getKey()) { //0 or 1 child node deletion
		keyFound = true;

		if (!(current->left != nullptr && current->right != nullptr)) {
			if (last->left == current) {
				if (current->right == nullptr) { last->left = current->left; }
				else if (current->left == nullptr) { last->left = current->right; }
			}
			else if (last->right == current) {
				if (current->right == nullptr) { last->right = current->left; }
				else if (current->left == nullptr) { last->right = current->right; }
			}
			else if (last == current) {
				if (root->right == nullptr) { root = root->left; }
				else if (root->left == nullptr) { root = root->right; }
			}
			delete current;
		}
		else { //2 child node deletion
			KeyType copyKey;
			retrieveMinKey(copyKey, current->right);
			removeHelper(copyKey, root, root);
			current->dataItem.setKey(copyKey);
		}
	}
	return keyFound;
}

/*
Another helper for remove function.
This function retrieves the minimum key in a node's subtree. This key is in the node
reached by searching leftward down the tree as far as possible. Its performance is
fast and consistent.
*/
template < class DataType, class KeyType >
void BSTree<DataType, KeyType>::retrieveMinKey(KeyType & searchKey, BSTreeNode * current) {
	if (current->left != nullptr) { retrieveMinKey(searchKey, current->left); }
	else { searchKey = current->dataItem.getKey(); }
}

/*
Helper for getCount function.
This function uses a recursive technique that searches both the left and right subtrees of
nodes, summing up the total number of nodes along the way. It does so without revisiting nodes
too many times and returns the total.
*/
template< typename DataType, typename KeyType >
int BSTree<DataType, KeyType>::countHelper(BSTreeNode * current) const {
	int totalNodes = 0;

	if (current != nullptr) {
		totalNodes = (1 + countHelper(current->left) + countHelper(current->right));
	}

	return totalNodes;
}

/*
Helper for getHeight method.
This function uses a recursive technique that searches both the left and right subtrees of
nodes, summing up the maximum height of the tree along the way. It does so without revisiting
nodes too many times and returns the total.
*/
template< typename DataType, typename KeyType >
int BSTree<DataType, KeyType>::heightHelper(BSTreeNode * current) const {
	if (current == nullptr) { return 0; }
	return max(heightHelper(current->left), heightHelper(current->right)) + 1;
}