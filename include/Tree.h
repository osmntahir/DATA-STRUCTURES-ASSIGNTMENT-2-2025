#ifndef TREE_H
#define TREE_H

#include "TreeNode.h"

class Tree {
private:
    TreeNode* root;

    void destroyTree(TreeNode*& node);
    void insertNode(TreeNode*& current, char c);
    int  getHeight(TreeNode* node);
    void mirrorHelper(TreeNode* node);

    // Ağaç ASCII çizimi
    void fillCanvas(TreeNode* node,
                    int row, int col,
                    char** canvas,
                    int rows, int cols,
                    int totalHeight);

public:
    Tree();
    ~Tree();

    void insert(char c);    // BST kuralına göre ekle
    void mirrorTree();      // En alttan swap approach
    int  calculateSum();    // (solAltToplam * 2) + sağAltToplam + kök
    void drawTree();        // Yukarıdan aşağıya ASCII
};

#endif
