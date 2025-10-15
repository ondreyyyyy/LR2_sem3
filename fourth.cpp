#include <iostream>
#include <string>
#include "array.h"

using namespace std;

DynArray generateSubarrays(const DynArray& arr) {
    DynArray result;
    createArr(result, 0);

    int n = genLength(arr);
    
    // Генерируем все возможные подмассивы используя битовые маски
    for (int mask = 0; mask < (1 << n); mask++) {
        string subarray = "{";
        bool first = true;
        
        for (int i = 0; i < n; i++) {
            if (mask & (1 << i)) {
                if (!first) {
                    subarray += ", ";
                }
                subarray += getElementIndex(i, arr);
                first = false;
            }
        }
        subarray += "}";
        pushBackArr(subarray, result);
    }

    return result;
}

int main() {
    cout << "Введите элементы массива через пробел: ";
    
    DynArray arr;
    createArr(arr, 0);
    
    string input;
    getline(cin, input);
    
    string element = "";
    for (char c : input) {
        if (c == ' ') {
            if (!element.empty()) {
                pushBackArr(element, arr);
                element = "";
            }
        } else {
            element += c;
        }
    }
    
    if (!element.empty()) {
        pushBackArr(element, arr);
    }
    
    cout << "Введенный массив: ";
    printArray(arr);
    
    DynArray res = generateSubarrays(arr);
    
    cout << "\nВсе подмассивы: ";
    printArray(res);
    
    return 0;
}