#ifndef HASHADDRESS_H
#define HASHADDRESS_H

#include <string>

using namespace std;

struct HANode {
    string key;
    int data;
    bool isDeleted;
    bool isEmpty;
};

struct HashA {
    HANode* table;
    int capacity;
    int size;
};

HashA* createHashATable();
int hashaFunc(const string& key, int capacity);
void addElementA(HashA* hasha, const string& key, int data);
int findElementA(HashA* hasha, const string& key);
void deleteElementA(HashA* hasha, const string& key);
void rehash(HashA* hasha);
void printHashTableA(HashA* hasha);

#endif