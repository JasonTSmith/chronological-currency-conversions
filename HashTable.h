#pragma once
#include <list>
#include <vector>
#include <string>
using namespace std;

// Hash table implemented with separate chaining to reduce collisions
class HashTable {
    int currItems;
    int capacity;
    float loadFactor;
    float maxLoadFactor;
    vector<list<pair<int, double>>> table;

    void resize(int newCapacity);
public:
    HashTable();
    HashTable(int capacity);
    HashTable(int capacity, float maxLoadFactor);

    bool isEmpty();
    int hash(int key);

    void insert(int key, double value);
    void remove(int key);
};

