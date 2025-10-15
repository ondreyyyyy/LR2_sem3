#ifndef TREE_H
#define TREE_H

#include <iostream>
#include <stdexcept>

using namespace std;

enum Color {
    RED,
    BLACK
};

struct huffmanNode {
    char letter;
    int frequency;
    huffmanNode* left;
    huffmanNode* right;
};

struct TNode {
    int key;
    huffmanNode* value; 
    Color color;
    TNode* left;
    TNode* right;
    TNode* parent;
};

struct Tree {
    TNode* root;
    TNode* nil;
};

Tree* createTree();
void removeTree(Tree* RBT);
void insert(Tree* RBT, int value);
void deleteNode(Tree* RBT, TNode* z);
TNode* findNode(Tree* RBT, int value);
void insertHuffman(Tree* RBT, int frequency, huffmanNode* huffNode);
TNode* findMinimum(Tree* RBT, TNode* node);
void printTree(Tree* RBT);
void saveTreeToStream(Tree* RBT, ostream& os);
void loadTreeFromStream(Tree* RBT, istream& is);

#endif