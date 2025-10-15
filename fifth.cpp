#include <iostream>
#include <vector>
#include "hashaddress.h"
#include "tree.h"

using namespace std;

HashA* countFreq(const string& input) {
    HashA* hasha = createHashATable();
    for (char c: input) {
        string keyS = string(1, c);
        int freq = findElementA(hasha, keyS);
        if (freq != -1) {
            addElementA(hasha, keyS, to_string(freq + 1));
        }
        else {
            addElementA(hasha, keyS, "1");
        }
    }

    return hasha;
}

Tree* createNodeList(HashA* hasha) {
    Tree* list = createTree();
    
    for (int i = 0; i < hasha->capacity; i++) {
        if (hasha->table[i].isEmpty == false && hasha->table[i].isDeleted == false) {
            string keySTR = hasha->table[i].key;
            char symbol = keySTR[0];
            int freq = stoi(hasha->table[i].data);

            huffmanNode* huffNode = new huffmanNode;
            huffNode->letter = symbol;
            huffNode->frequency = freq;
            huffNode->left = nullptr;
            huffNode->right = nullptr;

            insertHuffman(list, freq, huffNode);
        }
    }

    return list;
}

huffmanNode* getHuffmanTree(Tree* tree){
  while (tree->root->left != tree->nil || tree->root->right != tree->nil) {
        TNode* min1 = findMinimum(tree, tree->root);
        huffmanNode* huffmin1 = min1->value;
        int key1 = min1->key;
        deleteNode(tree, min1);
        
        TNode* min2 = findMinimum(tree, tree->root);
        huffmanNode* huffmin2 = min2->value;
        int key2 = min2->key;
        deleteNode(tree, min2);
        
        huffmanNode* Hparent = new huffmanNode;
        Hparent->frequency = huffmin1->frequency + huffmin2->frequency;
        
        if (huffmin1->frequency <= huffmin2->frequency) {
            Hparent->left = huffmin1;
            Hparent->right = huffmin2;
        } else {
            Hparent->left = huffmin2;
            Hparent->right = huffmin1;
        }
        
        insertHuffman(tree, Hparent->frequency, Hparent);
    }
  
    return tree->root->value;
}

void generateCode(huffmanNode* hufNode, string code, HashA* hasha) {
    if (hufNode == nullptr) {
        return;
    }

    if (hufNode->left == nullptr && hufNode->right == nullptr) {
        string keyS = string(1, hufNode->letter);
        addElementA(hasha, keyS, code);
        return;
    }

    if (hufNode->left != nullptr) {
        generateCode(hufNode->left, code + "0", hasha);
    }

    if (hufNode->right != nullptr) {
        generateCode(hufNode->right, code + "1", hasha);
    }
}

HashA* buildCode(huffmanNode* root) {
    HashA* codes = createHashATable();
    if (root != nullptr && root->left == nullptr && root->right == nullptr) {
        string keyS = string(1, root->letter);
        addElementA(codes, keyS, "0");
    }
    else {
        generateCode(root, "", codes);
    }
    return codes;
}

string encode(const string& input, HashA* codes) {
    string result = "";
    for (char c: input) {
        string keyS = string(1, c);
        string code = findElementAstr(codes, keyS);
    
        if (code != "-1") {
            result += code;
        }
    }

    return result;
}

string decode(huffmanNode* root, const string& encodedText) {
    string decoded = "";

    if (root->left == nullptr && root->right == nullptr) {
        for (size_t i = 0; i < encodedText.length(); i++) {
            decoded += root->letter;
        }
        return decoded;
    }

    huffmanNode* cur = root;
    for (char num: encodedText) {
        if (num == '0') cur = cur->left;
        else cur = cur->right; 
    
        if (cur-> left == nullptr && cur->right == nullptr) {
            decoded += cur->letter;
            cur = root;
        }
    }
    
    return decoded;
}

int main() {
    string input;
    cout << "=== Тестирование алгоритма Хаффмана ===" << endl;
    cout << "Введите строку для кодирования: ";
    getline(cin, input);
    
    if (input.empty()) {
        cout << "Ошибка: введена пустая строка!" << endl;
        return 1;
    }
    
    cout << "\n1. Подсчет частот символов:" << endl;
    HashA* frequencyTable = countFreq(input);
    for (int i = 0; i < frequencyTable->capacity; i++) {
        if (frequencyTable->table[i].isEmpty == false && frequencyTable->table[i].isDeleted == false) {
            cout << "Символ: '" << frequencyTable->table[i].key 
                 << "' Частота: " << frequencyTable->table[i].data << endl;
        }
    }
    
    cout << "\n2. Создание списка узлов для дерева Хаффмана..." << endl;
    Tree* nodeList = createNodeList(frequencyTable);
    
    cout << "\n3. Построение дерева Хаффмана:" << endl;
    huffmanNode* huffmanTree = getHuffmanTree(nodeList);
    if (huffmanTree == nullptr) {
        cout << "Ошибка: не удалось построить дерево Хаффмана!" << endl;
        return 1;
    }
    cout << "Дерево успешно построено. Корневая частота: " << huffmanTree->frequency << endl;
    
    cout << "\n4. Генерация кодов Хаффмана:" << endl;
    HashA* huffmanCodes = buildCode(huffmanTree);
    for (int i = 0; i < huffmanCodes->capacity; i++) {
        if (huffmanCodes->table[i].isEmpty == false && huffmanCodes->table[i].isDeleted == false) {
            cout << "Символ: '" << huffmanCodes->table[i].key 
                 << "' Код: " << huffmanCodes->table[i].data << endl;
        }
    }
    cout << endl;
    printHashTableA(huffmanCodes);
    
    cout << "\n5. Кодирование строки:" << endl;
    string encodedText = encode(input, huffmanCodes);
    cout << "Исходная строка: " << input << endl;
    cout << "Закодированная строка: " << encodedText << endl;
    
    cout << "\n6. Декодирование строки:" << endl;
    string decodedText = decode(huffmanTree, encodedText);
    cout << "Декодированная строка: " << decodedText << endl;
    
    cout << "\n7. Проверка корректности:" << endl;
    if (input == decodedText) {
        cout << "Кодирование и декодирование выполнены успешно!" << endl;
    } else {
        cout << "Ошибка: исходная и декодированная строки не совпадают!" << endl;
    }
    
    return 0;
}