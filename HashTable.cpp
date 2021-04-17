#include "HashTable.h"

HashTable::HashTable() {
    capacity = 100;
    table.resize(capacity);
    currItems = 0;
    loadFactor = currItems / capacity;
    maxLoadFactor = 4.0f;
}

HashTable::HashTable(int capacity) {
    this->capacity = capacity;
    table.resize(capacity);
    currItems = 0;
    loadFactor = currItems / capacity;
    maxLoadFactor = 4.0f;
}

HashTable::HashTable(int capacity, float maxLoadFactor) {
    this->capacity = capacity;
    table.resize(capacity);
    currItems = 0;
    loadFactor = currItems / capacity;
    this->maxLoadFactor = maxLoadFactor;
}

bool HashTable::isEmpty() {
    return (currItems == 0);
}

// Sums each digit of the date before using modulus with capacity
int HashTable::hash(int key) {
    int hash = 0;
    string keyString = to_string(key);
    for (int i = 0; i < keyString.length(); i++) {
        hash += (int) keyString[i];
    }
    return hash % capacity;
}

// Stores conversion rate in table per date
void HashTable::insert(int key, double value) {
    int hashedKey = hash(key);
    // Checks if date is already stored, overwrites if needed
    auto iter = table[hashedKey].begin();
    for (iter; iter != table[hashedKey].end(); iter++) {
        int date = iter->first;
        if (date == key) {
            iter->second = value;
            return;
        }
    }
    table[hashedKey].push_back(make_pair(key, value));
    currItems++;
}

// Finds key in table and deletes it
void HashTable::remove(int key) {
    int hashedKey = hash(key);
    auto iter = table[hashedKey].begin();
    for (iter; iter != table[hashedKey].end(); iter++) {
        int date = iter->first;
        if (date == key) {
            table[hashedKey].erase(iter);
            currItems--;
            return;
        }
    }
}

// Constructs new hash table of newCapacity size with previous values
void HashTable::resize(int newCapacity) {
    HashTable newTable(newCapacity);
    // For each bucket, rehash keys and store values
    for (int i = 0; i < table.size(); i++) {
        auto iter = table[i].begin();
        for (iter; iter != table[i].end(); iter++) {
            newTable.insert(iter->first, iter->second);
        }
    }
    *this = newTable;
}
