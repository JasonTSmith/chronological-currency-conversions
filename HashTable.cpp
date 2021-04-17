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
