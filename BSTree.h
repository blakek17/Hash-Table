//--------------------------------------------------------------------
//
//  Laboratory 9                                    BSTree.h
//
//  Class declarations for the linked implementation of the Binary
//  Search Tree ADT -- including the recursive helpers of the
//  public member functions
//
//--------------------------------------------------------------------

#ifndef BSTREE_H
#define BSTREE_H

#include <stdexcept>
#include <iostream>
using namespace std;

template < typename DataType, typename KeyType >    // DataType : tree data item
class BSTree                                     // KeyType : key field
{
public:
	// Constructor
	BSTree();                         // Default constructor
	BSTree(const BSTree<DataType, KeyType>& other);   // Copy constructor
	BSTree& operator= (const BSTree<DataType, KeyType>& other);
	// Overloaded assignment operator

// Destructor
	~BSTree();

	// Binary search tree manipulation operations
	void insert(const DataType& newDataItem);  // Insert data item
	bool retrieve(const KeyType& searchKey, DataType& searchDataItem);
	// Retrieve data item
	bool remove(const KeyType& deleteKey);            // Remove data item
	void writeKeys() const;                      // Output keys
	void clear();                                // Clear tree

	// Binary search tree status operations
	bool isEmpty() const;                        // Tree is empty
	// !! isFull() has been retired. Not very useful in a linked structure.

	// In-lab operations
	int getHeight() const;                       // Height of tree
	int getCount() const;			  // Number of nodes in tree
	//void writeLessThan(const KeyType& searchKey) const; // Output keys < searchKey

protected:
	class BSTreeNode                  // Inner class: facilitator for the BSTree class
	{
	public:
		// Constructor
		BSTreeNode(const DataType& nodeDataItem, BSTreeNode* leftPtr, BSTreeNode* rightPtr);

		// Data members
		DataType dataItem;         // Binary search tree data item
		BSTreeNode* left,    // Pointer to the left child
			* right;   // Pointer to the right child
	};
	// Data member
	BSTreeNode* root;   // Pointer to the root node

private:
	//Recursive helper functions
	void copyHelper(const BSTreeNode* otherCurrent);
	void insertHelper(const DataType& newDataItem, BSTreeNode*& current);
	bool retrieveHelper(const KeyType& searchKey, DataType& searchDataItem, BSTreeNode* current);
	void writeKeysHelper(BSTreeNode* current) const;
	void clearHelper(BSTreeNode* current);
	bool removeHelper(const KeyType& deleteKey, BSTreeNode* last, BSTreeNode* current);
	void retrieveMinKey(KeyType& searchKey, BSTreeNode* current);
	int countHelper(BSTreeNode* current) const;
	int heightHelper(BSTreeNode* current) const;
};

#endif	// define BSTREE_H