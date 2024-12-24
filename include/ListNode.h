#ifndef LISTNODE_H
#define LISTNODE_H

#include "Tree.h"

class ListNode {
public:
    Tree* tree;
    ListNode* next;

    ListNode(Tree* tree);
};

#endif
