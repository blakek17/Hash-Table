#include "HashTable.h"

template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::HashTable(int initTableSize) {
	tableSize = initTableSize;
	dataTable = new BSTree<DataType, KeyType>[tableSize];
	for (int i = 0; i < tableSize; i++) {
		dataTable[i] = BSTree<DataType, KeyType>();
	}
}

template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::HashTable(const HashTable& other) {
	tableSize = 0;
	dataTable = nullptr;
	*this = other;
}

template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>& HashTable<DataType, KeyType>::operator=(const HashTable& other) {
	clear();
	if (tableSize != other.tableSize) {
		delete [] dataTable;
		dataTable = nullptr;
		tableSize = other.tableSize;
		dataTable = new BSTree<DataType, KeyType>[tableSize];
	}
	for (int i = 0; i < tableSize; i++) {
		dataTable[i] = other.dataTable[i];
	}
	return *this;
}

template <typename DataType, typename KeyType>
HashTable<DataType, KeyType>::~HashTable() {
	delete [] dataTable;
	dataTable = nullptr;
}

template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::insert(const DataType& newDataItem) {
	KeyType key = newDataItem.getKey();
	unsigned int index = DataType::hash(key) % tableSize;
	dataTable[index].insert(newDataItem);
}

template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::remove(const KeyType& deleteKey) {
	unsigned int index = DataType::hash(deleteKey) % tableSize;
	return dataTable[index].remove(deleteKey);
}

template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::retrieve(const KeyType& searchKey, DataType& returnItem) {
	unsigned int index = DataType::hash(searchKey) % tableSize;
	return dataTable[index].retrieve(searchKey, returnItem);
}

template <typename DataType, typename KeyType>
void HashTable<DataType, KeyType>::clear() {
	for (int i = 0; i < tableSize; i++) {
		dataTable[i].clear();
	}
}

template <typename DataType, typename KeyType>
bool HashTable<DataType, KeyType>::isEmpty() const {
	for (int i = 0; i < tableSize; i++) {
		if (!dataTable[i].isEmpty())
			return false;
	}
	return true;
}