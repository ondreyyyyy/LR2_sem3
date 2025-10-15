#include <iostream>
#include "hashaddress.h"

using namespace std;

HashA* createHashATable() {
    HashA* hasha = new HashA;
    hasha->size = 0;
    hasha->capacity = 8;

    hasha->table = new HANode[hasha->capacity];
    for (int i = 0; i < hasha->capacity; i++) {
        hasha->table[i].isDeleted = false;
        hasha->table[i].isEmpty = true;
    }

    return hasha;
}

int hashaFunc(const string& key, int capacity) {
    int hash = 0;
    for (char c: key) {
        hash = (hash * 47 + c) % capacity;
    }

    return hash % capacity;
}

void rehash(HashA* hasha) {
    int oldCapacity = hasha->capacity;
    HANode* oldTable = hasha->table;
    
    hasha->capacity = oldCapacity * 2;
    hasha->table = new HANode[hasha->capacity];
    hasha->size = 0;
    
    for (int i = 0; i < hasha->capacity; i++) {
        hasha->table[i].isEmpty = true;
        hasha->table[i].isDeleted = false;
    }
    
    for (int i = 0; i < oldCapacity; i++) {
        if (!oldTable[i].isEmpty && !oldTable[i].isDeleted) {
            int j = 0;
            int index = hashaFunc(oldTable[i].key, hasha->capacity);
            int originalIndex = index;
            
            while (j < hasha->capacity) {
                if (hasha->table[index].isEmpty) {
                    hasha->table[index].key = oldTable[i].key;
                    hasha->table[index].data = oldTable[i].data;
                    hasha->table[index].isDeleted = false;
                    hasha->table[index].isEmpty = false;
                    hasha->size++;
                    break;
                }
                j++;
                index = (originalIndex + j) % hasha->capacity;
            }
        }
    }
    
    delete[] oldTable;
}


void addElementA(HashA* hasha, const string& key, int data) {
    int i = 0;
    if (hasha->size >= hasha->capacity * 0.75) {
        cout << "Требуется рехэширование, старый размер: " << hasha->capacity;
        rehash(hasha);
        cout << ", новый размер: " << hasha->capacity << endl;
    }
    
    int index = hashaFunc(key, hasha->capacity);
    int originalIndex = index;

    while (hasha->table[index].isEmpty == false && hasha->table[index].isDeleted == false) {
            if (hasha->table[index].key == key) {
                hasha->table[index].data = data;
                return;
            }
        i++; 
        index = (originalIndex + i) % hasha->capacity;
        
        if (i == hasha->capacity) {
            cout << "Нет места для вставки.\n";
            return;
        }
    }

    hasha->table[index].key = key;
    hasha->table[index].data = data;
    hasha->table[index].isDeleted = false;
    hasha->table[index].isEmpty = false;
    hasha->size++;
}

int findElementA(HashA* hasha, const string& key) {
    int i = 0;
    int index = hashaFunc(key, hasha->capacity);
    int originalIndex = index;

    while (i < hasha->capacity) {
        if (hasha->table[index].isEmpty == true && hasha->table[index].isDeleted == false) {
            return -1;
        }

        if (hasha->table[index].isEmpty == false && hasha->table[index].isDeleted == false) {
            if (hasha->table[index].key == key) {
                return hasha->table[index].data;
            }
        }

        i++; 
        index = (originalIndex + i) % hasha->capacity;
    }

    return -1;
}

void deleteElementA(HashA* hasha, const string& key) {
    int i = 0;
    int index = hashaFunc(key, hasha->capacity);
    int originalIndex = index;

    while (i < hasha->capacity) {
        if (hasha->table[index].isEmpty == true && hasha->table[index].isDeleted == false) {
            break;
        }

        if (hasha->table[index].isEmpty == false && hasha->table[index].isDeleted == false) {
            if (hasha->table[index].key == key) {
                hasha->table[index].isDeleted = true;
                hasha->size--;
                cout << "Элемент удален.\n";
                return;
            }
        }

        i++; 
        index = (originalIndex + i) % hasha->capacity;
    }

    cout << "Элемент не найден!\n";
}

void printHashTableA(HashA* hasha) {
    cout << "Хэш-таблица, размер: " << hasha->size << ", вместимость: " << hasha->capacity << "):" << endl;
    
    for (int i = 0; i < hasha->capacity; i++) {
        cout << "Ячейка " << i << ": ";
        
        if (hasha->table[i].isEmpty) {
            if (hasha->table[i].isDeleted) {
                cout << "[УДАЛЕНО]";
            } else {
                cout << "пусто";
            }
        } else {
            cout << "[" << hasha->table[i].key << " -> " << hasha->table[i].data << "]";
            if (hasha->table[i].isDeleted) {
                cout << " (удалено)";
            }
        }
        cout << endl;
    }
}