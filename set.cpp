#include <iostream>
#include "hashchain.h"
#include "set.h"

using namespace std;

Set* createSet() {
    Set* set = new Set;
    set->hashTable = createHashTable();
    return set;
}

void setInsert(Set* set, const string& elem) {
    addElement(set->hashTable, elem, 1);
}

void setRemove(Set* set, const string& elem) {
    deleteElement(set->hashTable, elem);
}

void printSet(Set* set) {
    cout << "{";
    bool firstElement = true;
    
    Hash* hash = set->hashTable;
    
    for (int i = 0; i < hash->capacity; i++) {
        HNode* current = hash->table[i];
        while (current != nullptr) {
            if (!firstElement) {
                cout << ", ";
            }
            cout << current->key;
            firstElement = false;
            current = current->next;
        }
    }
    
    cout << "}" << endl;
}

bool setContains(Set* set, const string& elem) {
    return findElement(set->hashTable, elem) != -1;
}