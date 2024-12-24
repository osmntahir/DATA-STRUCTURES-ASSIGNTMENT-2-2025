#include "Tree.h"
#include <iostream>
#include <cmath> // pow

Tree::Tree() : root(nullptr) {}

Tree::~Tree() {
    destroyTree(root);
}

void Tree::destroyTree(TreeNode*& node) {
    if (!node) return;
    destroyTree(node->left);
    destroyTree(node->right);
    delete node;
    node = nullptr;
}

void Tree::insertNode(TreeNode*& current, char c) {
    if (!current) {
        current = new TreeNode(c, static_cast<int>(c));
    } else {
        if (static_cast<int>(c) < current->asciiValue) {
            insertNode(current->left, c);
        } else {
            insertNode(current->right, c);
        }
    }
}

void Tree::insert(char c) {
    insertNode(root, c);
}

int Tree::calculateSum() {
    if (!root) return 0;

    struct SumRec {
        static int sum(TreeNode* n, bool isLeft) {
            if (!n) return 0;
            // Sol düğümse 2 ile çarp, sağ düğümse aynen ekle
            int nodeValue = isLeft ? (n->asciiValue * 2) : n->asciiValue;
            return nodeValue + sum(n->left, true) + sum(n->right, false);
        }
    };

    int leftSum = SumRec::sum(root->left, true);
    int rightSum = SumRec::sum(root->right, false);

    // Kök düğümün değerini ekle
    return root->asciiValue + leftSum + rightSum;
}

int Tree::getHeight(TreeNode* node) {
    if (!node) return 0;
    int lh = getHeight(node->left);
    int rh = getHeight(node->right);
    return 1 + (lh > rh ? lh : rh);
}

// Aynalama (post-order)
void Tree::mirrorHelper(TreeNode* node) {
    if (!node) return;
    mirrorHelper(node->left);
    mirrorHelper(node->right);

    TreeNode* tmp = node->left;
    node->left = node->right;
    node->right = tmp;
}

void Tree::mirrorTree() {
    mirrorHelper(root);
}

// Ağaç ASCII çizimi
void Tree::fillCanvas(TreeNode* node,
                      int row, int col,
                      char** canvas,
                      int rows, int cols,
                      int totalHeight)
{
    if (!node) return;
    if (row >= 0 && row < rows && col >= 0 && col < cols) {
        canvas[row][col] = node->data;
    }

    int currLevel = row / 2;
    int remain = totalHeight - (currLevel + 1);

    int gap = 1;
    if (remain > 0) {
        gap = (int)std::pow(2, remain - 1);
        if (gap < 1) gap = 1;
    }

    // sol
    if (node->left) {
        for (int c = col - 1; c >= col - gap && c >= 0; c--) {
            if (row+1 < rows && c < cols) {
                canvas[row+1][c] = '.';
            }
        }
        fillCanvas(node->left, row+2, col-gap, canvas, rows, cols, totalHeight);
    }
    // sağ
    if (node->right) {
        for (int c = col + 1; c <= col + gap && c < cols; c++) {
            if (row+1 < rows && c < cols) {
                canvas[row+1][c] = '.';
            }
        }
        fillCanvas(node->right, row+2, col+gap, canvas, rows, cols, totalHeight);
    }
}

void Tree::drawTree() {
    int h = getHeight(root);
    if (h == 0) {
        std::cout << "[Bos Agac]\n\n";
        return;
    }

    int rows = 2*h - 1;
    int cols = (int)std::pow(2, h+1);

    char** canvas = new char*[rows];
    for (int r = 0; r < rows; r++) {
        canvas[r] = new char[cols];
        for (int c = 0; c < cols; c++) {
            canvas[r][c] = ' ';
        }
    }

    int startCol = cols / 2;
    fillCanvas(root, 0, startCol, canvas, rows, cols, h);

    for (int r = 0; r < rows; r++) {
        int end = cols - 1;
        while (end >= 0 && canvas[r][end] == ' ') {
            end--;
        }
        for (int c = 0; c <= end; c++) {
            std::cout << canvas[r][c];
        }
        std::cout << "\n";
    }
    std::cout << "\n";

    for (int r = 0; r < rows; r++) {
        delete[] canvas[r];
    }
    delete[] canvas;
}
