#ifndef TREENODE_H
#define TREENODE_H

class TreeNode {
public:
    char data;         // Karakter
    int  asciiValue;   // ASCII değeri
    TreeNode* left;
    TreeNode* right;

    TreeNode(char data, int asciiValue);
};

#endif
