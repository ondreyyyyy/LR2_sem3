#include <iostream>
#include <vector>
#include <limits>
#include "doubleFL.h"
#include "hashchain.h"

using namespace std;


struct LRUCache {
    int capacity;
    Hash* hashTable;
    doubleFL* accessOrder;
};

LRUCache* createLRUCache(int capacity) {
    LRUCache* cache = new LRUCache;
    cache->capacity = capacity;
    cache->hashTable = createHashTable();
    cache->accessOrder = createDoubleFL();
    return cache;
}

string intToString(int key) {
    return to_string(key);
}

int stringToInt(const string& str) {
    return stoi(str);
}

void set(LRUCache* cache, int key, int data) {
    string keyString = intToString(key);

    if (findElement(cache->hashTable, keyString) != -1) {
        addElement(cache->hashTable, keyString, data);
        deleteValue(*(cache->accessOrder), keyString);
        addAtHead(*(cache->accessOrder), keyString);
    }
    else {
        if (cache->hashTable->size >= cache->capacity) {
            string oldestKey = cache->accessOrder->tail->value;
            deleteAtTail(*(cache->accessOrder));
            deleteElement(cache->hashTable, oldestKey);
        }

        addElement(cache->hashTable, keyString, data);
        addAtHead(*(cache->accessOrder), keyString);
    }
}

int get(LRUCache* cache, int key) {
    string keyString = intToString(key);
    int data = findElement(cache->hashTable, keyString);

    if (data != -1) {
        deleteValue(*(cache->accessOrder), keyString);
        addAtHead(*(cache->accessOrder), keyString);
        return data;
    }
    else {
        return -1;
    }
}

int main() {
    int cap, Q;
    
    cout << "Введите вместимость кэша: ";
    while (!(cin >> cap) || cap <= 0) {
        cout << "Ошибка! Введите положительное целое число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    cout << "Введите количество команд: ";
    while (!(cin >> Q) || Q <= 0) {
        cout << "Ошибка! Введите положительное целое число: ";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    
    LRUCache* cache = createLRUCache(cap);
    
    cout << "Введите " << Q << " команд:" << endl;
    
    vector<int> results;
    
    for (int i = 0; i < Q; i++) {
        string command;
        cin >> command;
        
        if (command == "SET") {
            int x, y;
            if (cin >> x >> y) {
                set(cache, x, y);
            } else {
                cout << "Ошибка! Неверный формат для SET." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                i--; 
            }
        } 
        else if (command == "GET") {
            int x;
            if (cin >> x) {
                int result = get(cache, x);
                results.push_back(result);
            } else {
                cout << "Ошибка! Неверный формат для GET." << endl;
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                i--; 
            }
        }
        else {
            cout << "Ошибка! Неизвестная команда: " << command << "." << endl;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            i--; 
        }
    }
    
    if (!results.empty()) {
        cout << "Вывод: ";
        for (size_t i = 0; i < results.size(); i++) {
            if (i > 0) cout << " ";
            cout << results[i];
        }
        cout << endl;
    } else {
        cout << "Нет результатов GET операций для вывода." << endl;
    }
    
    return 0;
}