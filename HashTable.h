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

    int hash(int key);
    void resize(int newCapacity);
    bool updateLoadFactor();
public:
    HashTable();
    HashTable(int capacity);
    HashTable(int capacity, float maxLoadFactor);

    bool isEmpty();
    void insert(int key, double value);
    void remove(int key);

    double operator[](int key);

    float getCurrLoadFactor();
    float getMaxLoadFactor();
    int getCurrItems();
    int getNumBuckets();
};

