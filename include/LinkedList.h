#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "ListNode.h"

class LinkedList {
private:
    ListNode* head;
    ListNode* tail;

    // Yardımcı fonksiyonlar
    static char* pointerToHexLast4(void* ptr);
    static void centerText(const char* src, int width, char* dest);

public:
    LinkedList();
    ~LinkedList();

    void append(Tree* newTree);
    void removeNode(ListNode* node);

    ListNode* getHead() const;
    int getSize() const;

    // Listeyi maxCount adet (default 10) düğümle çiz, startIndex'ten başlayarak
    // Seçili düğüm (cursor) altına ^^^^^^^^ ve |||||||| ekler
    void drawLinkedList(ListNode* selected, int startIndex, int maxCount = 10);
};

#endif
