#include "LinkedList.h"
#include <iostream>
#include <sstream>
#include <cstring> // strncpy, memset, strcat, sprintf

LinkedList::LinkedList() 
{
    head = tail = nullptr;
}

LinkedList::~LinkedList() {
    ListNode* cur = head;
    while (cur) {
        ListNode* temp = cur;
        cur = cur->next;
        delete temp->tree;
        delete temp;
    }
    head = tail = nullptr;
}

void LinkedList::append(Tree* newTree) {
    ListNode* node = new ListNode(newTree);
    if (!head) {
        head = tail = node;
    } else {
        tail->next = node;
        tail = node;
    }
}

void LinkedList::removeNode(ListNode* node) {
    if (!node || !head) return;

    if (node == head) {
        head = head->next;
        if (!head) tail = nullptr;
        delete node->tree;
        delete node;
        return;
    }

    ListNode* prev = head;
    while (prev->next && prev->next != node) {
        prev = prev->next;
    }
    if (prev->next == node) {
        prev->next = node->next;
        if (node == tail) {
            tail = prev;
        }
        delete node->tree;
        delete node;
    }
}

ListNode* LinkedList::getHead() const {
    return head;
}

int LinkedList::getSize() const {
    int count = 0;
    ListNode* temp = head;
    while (temp) {
        count++;
        temp = temp->next;
    }
    return count;
}

// Adresin son 3-4 hex hanesi
char* LinkedList::pointerToHexLast4(void* ptr) {
    if (!ptr) {
        static char nullStr[5] = "NULL";
        return nullStr;
    }
    static char result[16];
    std::ostringstream oss;
    oss << ptr; // "0x7ffd..."
    std::string full = oss.str();
    if (full.size() <= 2) {
        strncpy(result, full.c_str(), 15);
        result[15] = '\0';
        return result;
    }
    std::string hexPart = full.substr(2);
    if (hexPart.size() > 4) {
        hexPart = hexPart.substr(hexPart.size() - 4);
    }
    strncpy(result, hexPart.c_str(), 15);
    result[15] = '\0';
    return result;
}

// 'src' metnini 'width' karakter içinde ortalayarak 'dest'e yazar
void LinkedList::centerText(const char* src, int width, char* dest) {
    int len = 0;
    while (src[len] != '\0') {
        len++;
    }
    if (len >= width) {
        // Sığmıyorsa kes
        strncpy(dest, src, width);
        dest[width] = '\0';
        return;
    }
    int left = (width - len) / 2;
    int idx = 0;
    // solda left kadar boşluk
    for (int i = 0; i < left; i++) {
        dest[idx++] = ' ';
    }
    // metin
    for (int i = 0; i < len; i++) {
        dest[idx++] = src[i];
    }
    // sağ kalan boşluk
    while (idx < width) {
        dest[idx++] = ' ';
    }
    dest[idx] = '\0';
}

/*
    Her düğüm 8 karakterlik bir kutu + 3 boşluk = 11 sütunluk segment. 
    7 satır kutu + (seçiliyse) 2 satır ek:
       1) ........
       2) .ADDR.. 
       3) ........
       4) .SUM...
       5) ........
       6) .NEXT..
       7) ........
       8) ^^^^^^^^ (varsa)
       9) |||||||| (varsa)
    Aralarda 3 karakterlik boşluk ekleyerek yan yana yerleştireceğiz.
*/
void LinkedList::drawLinkedList(ListNode* selected, int startIndex, int maxCount) {
    int total = getSize();
    if (total == 0) {
        std::cout << "[Liste bos]\n\n";
        return;
    }
    if (startIndex < 0) startIndex = 0;
    if (startIndex >= total) startIndex = 0;

    int endIndex = startIndex + maxCount;
    if (endIndex > total) endIndex = total;

    ListNode* temp = head;
    int idx = 0;
    while (temp && idx < startIndex) {
        temp = temp->next;
        idx++;
    }

    int realCount = endIndex - startIndex;
    if (realCount <= 0) {
        std::cout << "[Gosterilecek dugum yok]\n\n";
        return;
    }

    // Her düğüm 8 kolon, arada 3 boşluk => 11 kolon x realCount (toplam)
    const int BOX_WIDTH = 8; // her kutunun genişliği 8
    const int BOX_SPACE = 3; // düğümler arası boşluk
    int lineLen = realCount * (BOX_WIDTH + BOX_SPACE);

    // 9 satırı (kutu + seçili satırlar) hafızada tutacağız
    // Düğüm sayısı kadar yanyana ekleyeceğiz
    char* line1 = new char[lineLen + 1];
    char* line2 = new char[lineLen + 1];
    char* line3 = new char[lineLen + 1];
    char* line4 = new char[lineLen + 1];
    char* line5 = new char[lineLen + 1];
    char* line6 = new char[lineLen + 1];
    char* line7 = new char[lineLen + 1];
    char* line8 = new char[lineLen + 1];
    char* line9 = new char[lineLen + 1];

    auto initLine = [&](char* line) {
        line[0] = '\0';
    };
    initLine(line1);
    initLine(line2);
    initLine(line3);
    initLine(line4);
    initLine(line5);
    initLine(line6);
    initLine(line7);
    initLine(line8);
    initLine(line9);

    // Yardımcı: Bir satırı (8 kolonluk kutu) ve yanındaki 3 boşluk üret
    auto appendSegment = [&](char* destLine, const char* segment) {
        strcat(destLine, segment);
        // araya 3 boşluk ekle
        int len = (int)strlen(destLine);
        destLine[len] = ' ';
        destLine[len + 1] = ' ';
        destLine[len + 2] = ' ';
        destLine[len + 3] = '\0';
    };

    for (int i = 0; i < realCount && temp; i++) {
        // Bilgiler
        char addrCur[16] = {0}, sumStr[16] = {0}, addrNext[16] = {0};

        // Adres
        char* sAddr = pointerToHexLast4((void*)temp);
        strncpy(addrCur, sAddr, 15);

        // Sum
        int sumVal = temp->tree ? temp->tree->calculateSum() : 0;
        sprintf(sumStr, "%d", sumVal);

        // Next
        if (temp->next) {
            char* sN = pointerToHexLast4((void*)temp->next);
            strncpy(addrNext, sN, 15);
        } else {
            strncpy(addrNext, "NULL", 15);
        }

        // 7 satır kutu + 2 satır seçili
        // Her satır 8 karakter (BOX_WIDTH)
        // 1) ........
        // 2) .xxxxxx.
        // 3) ........
        // 4) .xxxxxx.
        // 5) ........
        // 6) .xxxxxx.
        // 7) ........
        // 8) ^^^^^^^^ (varsa)
        // 9) |||||||| (varsa)

        // Geçici buffer
        char seg1[BOX_WIDTH + 1], seg2[BOX_WIDTH + 1], seg3[BOX_WIDTH + 1];
        char seg4[BOX_WIDTH + 1], seg5[BOX_WIDTH + 1], seg6[BOX_WIDTH + 1];
        char seg7[BOX_WIDTH + 1], seg8[BOX_WIDTH + 1], seg9[BOX_WIDTH + 1];

        // Hepsini başta null-terminate yapalım
        memset(seg1, 0, sizeof(seg1));
        memset(seg2, 0, sizeof(seg2));
        memset(seg3, 0, sizeof(seg3));
        memset(seg4, 0, sizeof(seg4));
        memset(seg5, 0, sizeof(seg5));
        memset(seg6, 0, sizeof(seg6));
        memset(seg7, 0, sizeof(seg7));
        memset(seg8, 0, sizeof(seg8));
        memset(seg9, 0, sizeof(seg9));

        // 1,3,5,7 => "........"
        strncpy(seg1, "........", BOX_WIDTH);
        strncpy(seg3, "........", BOX_WIDTH);
        strncpy(seg5, "........", BOX_WIDTH);
        strncpy(seg7, "........", BOX_WIDTH);

        // 2 => "." + centerText(addrCur,6) + "."
        {
            char middle[7];
            centerText(addrCur, 6, middle); // 6 karakter ortada
            seg2[0] = '.';
            strncpy(seg2 + 1, middle, 6);
            seg2[7] = '.';
            seg2[8] = '\0';
        }

        // 4 => "." + centerText(sumStr,6) + "."
        {
            char middle[7];
            centerText(sumStr, 6, middle);
            seg4[0] = '.';
            strncpy(seg4 + 1, middle, 6);
            seg4[7] = '.';
            seg4[8] = '\0';
        }

        // 6 => "." + centerText(addrNext,6) + "."
        {
            char middle[7];
            centerText(addrNext, 6, middle);
            seg6[0] = '.';
            strncpy(seg6 + 1, middle, 6);
            seg6[7] = '.';
            seg6[8] = '\0';
        }

        // 8 => seçiliyse "^^^^^^^^", değilse "        "
        if (temp == selected) {
            strncpy(seg8, "^^^^^^^^", BOX_WIDTH);
        } else {
            strncpy(seg8, "        ", BOX_WIDTH);
        }

        // 9 => seçiliyse "||||||||", değilse "        "
        if (temp == selected) {
            strncpy(seg9, "||||||||", BOX_WIDTH);
        } else {
            strncpy(seg9, "        ", BOX_WIDTH);
        }

        // Şimdi bu 9 satır segmentlerini line1..line9'a ekle + 3 boşluk
        appendSegment(line1, seg1);
        appendSegment(line2, seg2);
        appendSegment(line3, seg3);
        appendSegment(line4, seg4);
        appendSegment(line5, seg5);
        appendSegment(line6, seg6);
        appendSegment(line7, seg7);
        appendSegment(line8, seg8);
        appendSegment(line9, seg9);

        // Sonraki düğüme geç
        temp = temp->next;
    }

    // Ekrana bas
    std::cout << line1 << "\n";
    std::cout << line2 << "\n";
    std::cout << line3 << "\n";
    std::cout << line4 << "\n";
    std::cout << line5 << "\n";
    std::cout << line6 << "\n";
    std::cout << line7 << "\n";
    std::cout << line8 << "\n";
    std::cout << line9 << "\n\n";

    // Bellek temizliği
    delete[] line1; delete[] line2; delete[] line3;
    delete[] line4; delete[] line5; delete[] line6;
    delete[] line7; delete[] line8; delete[] line9;
}
