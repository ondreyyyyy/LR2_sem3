#ifndef HASHCHAIN_H
#define HASHCHAIN_H

#include <string>

using namespace std;

struct HNode {
    string key;
    int data;
    HNode* next;
    HNode* prev;
};

struct Hash {
    HNode** table;
    int size;
    int capacity;
};

Hash* createHashTable();
void addElement(Hash* hash, const string& key, int data);
int findElement(Hash* hash, const string& key);
void deleteElement(Hash* hash, const string& key);
void printHashTable(Hash* hash);

#endif // HASHCHAIN_H