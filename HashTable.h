#pragma once
#include <list>
#include <vector>
#include <string>
using namespace std;

// Hash table implemented with separate chaining to reduce collisions
class HashTable {
    int capacity;
    float loadFactor;
    vector<list<pair<int, double>>> table;

    void resize();
public:
    HashTable();
    HashTable(int capacity);

    bool isEmpty();
    int hash(int key);

    void insert(int key, double value);
    void remove(int key);
    string search(int key);
};

