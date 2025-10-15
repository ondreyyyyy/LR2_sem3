#include <iostream>
#include <stdexcept>
#include "hashchain.h"

using namespace std;

Hash* createHashTable() {
    Hash* hash = new Hash;
    hash->size = 0;
    hash->capacity = 8;

    hash->table = new HNode*[hash->capacity];
    for (int i = 0; i < hash->capacity; i++) {
        hash->table[i] = nullptr;
    }

    return hash;
}

int hashFunc(const string& key, int capacity) {
    int hash = 0;
    for (char c: key) {
        hash = (hash * 47 + c) % capacity;
    }

    return hash % capacity;
}

void addElement(Hash* hash, const string& key, int data) {
    int index = hashFunc(key, hash->capacity);
    HNode* begin = hash->table[index];
    HNode* curElem = begin;

    while (curElem != nullptr) {
        if (curElem->key == key) {
            curElem->data = data;
            return;
        }
        curElem = curElem->next;
    }

    HNode* addNode = new HNode;
    addNode->key = key;
    addNode->data = data;
    addNode->next = begin;
    addNode->prev =  nullptr;
    if (hash->table[index] != nullptr) {
        hash->table[index]->prev = addNode;
    }
    
    hash->table[index] = addNode;
    hash->size++;
}

int findElement(Hash* hash, const string& key) {
    int index = hashFunc(key, hash->capacity);
    HNode* begin = hash->table[index];
    HNode* curElem = begin;

    while (curElem != nullptr) {
        if (curElem->key == key) {
            return curElem->data;
        }
        curElem = curElem->next;
    }
    
    return -1;
}

void deleteElement(Hash* hash, const string& key) {
    int index = hashFunc(key, hash->capacity);
    HNode* deleteNode = hash->table[index];

    while (deleteNode != nullptr) {
        if (deleteNode->key == key) {
            if (deleteNode->prev != nullptr) {
                deleteNode->prev->next = deleteNode->next;
            }
            else {
                hash->table[index] = deleteNode->next;
            }
            if (deleteNode->next != nullptr) {
                deleteNode->next->prev = deleteNode->prev;
            }
            delete deleteNode;
            hash->size--;
            return;
        }
        deleteNode = deleteNode->next;
    }
}

void printHashTable(Hash* hash) {
    cout << "Хэш-таблица (элементов: " << hash->size << ", вместимость: " << hash->capacity << "):" << endl;
    
    for (int i = 0; i < hash->capacity; i++) {
        cout << "Цепочка " << i << ": ";
        
        HNode* current = hash->table[i];
        if (current == nullptr) {
            cout << "пусто";
        } else {
            while (current != nullptr) {
                cout << "[" << current->key << " -> " << current->data << "]";
                if (current->next != nullptr) {
                    cout << " -> ";
                }
                current = current->next;
            }
        }
        cout << endl;
    }
}