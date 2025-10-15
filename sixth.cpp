#include <iostream>
#include <string>
#include <cmath>

using namespace std;

int convolutionHash(const string& key, int numParts) {
    int hash = 0;
    int n = key.length();
    int partSize = ceil((double)n / numParts);
    
    for (int i = 0; i < n; i += partSize) {
        int partValue = 0;
        int end = min(i + partSize, n);
        
        for (int j = i; j < end; j++) {
            partValue = partValue * 10 + (key[j] - '0');
        }
        
        hash += partValue;
    }
    
    return hash;
}

int main() {
    string key;
    int numParts;
    
    cout << "Введите ключ (число): ";
    cin >> key;
    
    for (char c : key) {
        if (!isdigit(c)) {
            cout << "Ошибка: ключ должен содержать только цифры!" << endl;
            return 1;
        }
    }
    
    if (key.empty()) {
        cout << "Ошибка: ключ не может быть пустым!" << endl;
        return 1;
    }
    
    cout << "Введите количество частей для разбиения: ";
    cin >> numParts;
    
    if (numParts < 1) {
        cout << "Ошибка: количество частей должно быть не менее 1!" << endl;
        return 1;
    }
    
    if (numParts > key.length()) {
        cout << "Ошибка: количество частей не может быть больше длины ключа!" << endl;
        return 1;
    }
    
    int partSize = ceil((double)key.length() / numParts);
    
    cout << "Количество частей: " << numParts << endl;
    cout << "Размер каждой части: " << partSize << " цифр(ы)" << endl;
    
    int hash = convolutionHash(key, numParts);
    
    cout << "Сумма частей: ";
    int n = key.length();
    for (int i = 0; i < n; i += partSize) {
        int end = min(i + partSize, n);
        string part = key.substr(i, end - i);
        if (i > 0) cout << " + ";
        cout << part;
    }
    cout << " = " << hash << endl;
    
    cout << "Финальный хеш: " << hash << endl;
    
    return 0;
}