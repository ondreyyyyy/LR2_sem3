#ifndef SET_H
#define SET_H

#include <string>
#include "hashchain.h"

using namespace std;

struct Set {
    Hash* hashTable; 
};

Set* createSet();
void setInsert(Set* set, const string& elem);
void setRemove(Set* set, const string& elem);
bool setContains(Set* set, const string& elem);
void printSet(Set* set);

#endif // SET_H