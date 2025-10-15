#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include <fstream>
#include "set.h"
#include "hashchain.h"

using namespace std;

map<string, Set*> sets;

void saveToFile(const string& filename);
void loadFromFile(const string& filename);
void processSetCommand(const vector<string>& args);
void processPrintCommand(const vector<string>& args);
vector<string> split(const string& str);
void printHelp();

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Ошибка: неверное количество аргументов. Используйте --help для справки." << endl;
        return 1;
    }
    
    string filename;
    string query;
    
    for (int i = 1; i < argc; i++) {
        string arg = argv[i];
        if (arg == "--file" && i + 1 < argc) {
            filename = argv[++i];
        }
        else if (arg == "--query" && i + 1 < argc) {
            query = argv[++i];
        }
        else if (arg == "--help" || arg == "-h") {
            printHelp();
            return 0;
        }
        else {
            cerr << "Ошибка: неизвестный аргумент " << arg << endl;
            return 1;
        }
    }
    
    if (filename.empty() || query.empty()) {
        cerr << "Ошибка: необходимо указать --file и --query" << endl;
        return 1;
    }
    
    loadFromFile(filename);
    
    vector<string> args = split(query);
    if (args.empty()) {
        cerr << "Ошибка: пустой запрос" << endl;
        return 1;
    }
    
    string command = args[0];
    
    if (command == "SETCREATE" || command == "SETADD" || command == "SETDEL" || 
        command == "SET_AT") {
        processSetCommand(args);
    }
    else if (command == "PRINT") {
        processPrintCommand(args);
    }
    else if (command == "HELP") {
        printHelp();
    }
    else {
        cerr << "Неизвестная команда: " << command << endl;
        return 1;
    }
    
    saveToFile(filename);
    
    for (auto& pair : sets) {
        delete pair.second;
    }
    
    return 0;
}

void processSetCommand(const vector<string>& args) {
    if (args.size() < 2) {
        cerr << "Ошибка: недостаточно аргументов для команды " << args[0] << endl;
        return;
    }
    
    string name = args[1];
    
    try {
        if (args[0] == "SETCREATE") {
            if (args.size() != 2) {
                cerr << "Ошибка: неверное количество аргументов для SETCREATE" << endl;
                return;
            }
            if (sets.find(name) != sets.end()) {
                cerr << "Ошибка: множество '" << name << "' уже существует" << endl;
                return;
            }
            sets[name] = createSet();
            cout << "Создание множества '" << name << "' успешно" << endl;
        }
        else if (args[0] == "SETADD") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для SETADD" << endl;
                return;
            }
            if (sets.find(name) == sets.end()) {
                cerr << "Ошибка: множество '" << name << "' не существует" << endl;
                return;
            }
            setInsert(sets[name], args[2]);
            cout << "Добавление элемента '" << args[2] << "' в множество '" << name << "' успешно" << endl;
        }
        else if (args[0] == "SETDEL") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для SETDEL" << endl;
                return;
            }
            if (sets.find(name) == sets.end()) {
                cerr << "Ошибка: множество '" << name << "' не существует" << endl;
                return;
            }
            setRemove(sets[name], args[2]);
            cout << "Удаление элемента '" << args[2] << "' из множества '" << name << "' успешно" << endl;
        }
        else if (args[0] == "SET_AT") {
            if (args.size() != 3) {
                cerr << "Ошибка: неверное количество аргументов для SET_AT" << endl;
                return;
            }
            if (sets.find(name) == sets.end()) {
                cerr << "Ошибка: множество '" << name << "' не существует" << endl;
                return;
            }
            bool contains = setContains(sets[name], args[2]);
            if (contains) {
                cout << "Элемент '" << args[2] << "' найден в множестве '" << name << "'" << endl;
            } else {
                cout << "Элемент '" << args[2] << "' не найден в множестве '" << name << "'" << endl;
            }
        }
    } catch (const exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void processPrintCommand(const vector<string>& args) {
    if (args.size() != 2) {
        cerr << "Ошибка: неверное количество аргументов для PRINT. Ожидается: PRINT <name>" << endl;
        return;
    }
    
    string name = args[1];
    
    if (sets.find(name) != sets.end()) {
        cout << "Содержимое множества '" << name << "': ";
        printSet(sets[name]);
    }
    else {
        cerr << "Ошибка: множество с именем '" << name << "' не найдено" << endl;
    }
}

vector<string> getAllElementsFromHashTable(Hash* hashTable) {
    vector<string> elements;
    if (!hashTable) return elements;
    
    for (int i = 0; i < hashTable->capacity; i++) {
        HNode* current = hashTable->table[i];
        while (current != nullptr) {
            elements.push_back(current->key);
            current = current->next;
        }
    }
    return elements;
}

void saveToFile(const string& filename) {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Ошибка: не удалось открыть файл для записи" << endl;
        return;
    }
    
    for (const auto& pair : sets) {
        file << "SET " << pair.first;
        
        vector<string> elements = getAllElementsFromHashTable(pair.second->hashTable);
        
        for (const string& elem : elements) {
            file << " " << elem;
        }
        file << endl;
    }
    
    file.close();
}

void loadFromFile(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }
    
    string line;
    
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        stringstream ss(line);
        string type, name;
        ss >> type >> name;
        
        if (type == "SET") {
            if (sets.find(name) == sets.end()) {
                sets[name] = createSet();
            }
            
            string element;
            while (ss >> element) {
                setInsert(sets[name], element);
            }
        }
    }
    
    file.close();
}

vector<string> split(const string& str) {
    vector<string> result;
    stringstream ss(str);
    string item;
    
    while (ss >> item) {
        if (!item.empty() && item.front() == '\'' && item.back() == '\'') {
            item = item.substr(1, item.size() - 2);
        }
        result.push_back(item);
    }
    
    return result;
}

void printHelp() {
    cout << "=== СПРАВКА ПО КОМАНДАМ ===" << endl;
    cout << "\nМНОЖЕСТВА:" << endl;
    cout << "  SETCREATE <name>          - Создать множество" << endl;
    cout << "  SETADD <name> <value>     - Добавить элемент в множество" << endl;
    cout << "  SETDEL <name> <value>     - Удалить элемент из множества" << endl;
    cout << "  SET_AT <name> <value>     - Проверить наличие элемента в множестве" << endl;
    
    cout << "\nОБЩИЕ КОМАНДЫ:" << endl;
    cout << "  PRINT <name>              - Показать содержимое множества" << endl;
    cout << "  HELP                      - Справка" << endl;
    
    cout << "\nПРИМЕРЫ ЗАПУСКA:" << endl;
    cout << "  ./<program> --file data.txt --query 'SETCREATE myset'" << endl;
    cout << "  ./<program> --file data.txt --query 'SETADD myset apple'" << endl;
    cout << "  ./<program> --file data.txt --query 'SETADD myset banana'" << endl;
    cout << "  ./<program> --file data.txt --query 'SET_AT myset apple'" << endl;
    cout << "  ./<program> --file data.txt --query 'PRINT myset'" << endl;
    cout << "  ./<program> --help" << endl;
}