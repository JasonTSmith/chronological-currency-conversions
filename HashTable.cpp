#include "HashTable.h"

HashTable::HashTable() {
    capacity = 100;
    table.resize(capacity);
    loadFactor = table.size() / table.capacity();
}

HashTable::HashTable(int capacity) {
    this->capacity = capacity;
    table.resize(capacity);
    loadFactor = table.size() / table.capacity();
}

bool HashTable::isEmpty() {
    for (int i = 0; i < table.size(); i++) {
        if (!table[i].empty())
            return false;
    }
    return true;
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
}
