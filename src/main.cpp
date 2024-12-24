#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>   // Windows getch()
#include "LinkedList.h"

int main() {
    std::ifstream file("./doc/agaclar.txt");
    if (!file.is_open()) {
        std::cerr << "Dosya acilamadi!\n";
        return 1;
    }

    LinkedList list;
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            Tree* newTree = new Tree();
            for (char c : line) {
                newTree->insert(c);
            }
            list.append(newTree);
        }
    }
    file.close();

    int selectedIndex = 0;
    int startIndex = 0;
    const int pageSize = 10;

    auto getSizeSafe = [&]() { return list.getSize(); };

    auto getNodeByIndex = [&](int idx) -> ListNode* {
        int sz = getSizeSafe();
        if (idx < 0 || idx >= sz) return nullptr;
        ListNode* p = list.getHead();
        for (int i = 0; i < idx; i++) {
            p = p->next;
        }
        return p;
    };

    int listSize = getSizeSafe();
    ListNode* selected = getNodeByIndex(selectedIndex);

    bool running = true;
    while (running) {
        system("cls"); // Windows ekran temizleme

        std::cout << "===== BAGLI LISTE =====\n";

        if (listSize > 0) {
            list.drawLinkedList(selected, startIndex, pageSize);

         
            if (selected && selected->tree) {
                selected->tree->drawTree();
                std::cout << "Toplam Deger: " << selected->tree->calculateSum() << "\n\n";
            } else {
                std::cout << "[Secili dugum yok]\n\n";
            }
        } else {
            std::cout << "[Listede hic dugum kalmadi]\n\n";
        }

        std::cout << "Komutlar: [A] Geri, [D] Ileri, [S] Sil, [W] Aynala, [Q] Cikis\n";
        std::cout << "Secim: ";
        char ch = getch();
        std::cout << ch << "\n";

        switch (ch) {
        case 'A':
        case 'a': {
            if (listSize > 0 && selectedIndex > 0) {
                selectedIndex--;
                if (selectedIndex < startIndex) {
                    startIndex -= pageSize;
                    if (startIndex < 0) startIndex = 0;
                }
            }
        } break;
        case 'D':
        case 'd': {
            if (listSize > 0 && selectedIndex < listSize - 1) {
                selectedIndex++;
                if (selectedIndex >= startIndex + pageSize) {
                    startIndex += pageSize;
                    if (startIndex >= listSize) {
                        startIndex = (listSize / pageSize) * pageSize;
                        if (startIndex >= listSize) {
                            startIndex = 0;
                        }
                    }
                }
            }
        } break;
        case 'S':
        case 's': {
            if (listSize > 0) {
                ListNode* toDel = getNodeByIndex(selectedIndex);
                if (toDel) {
                    list.removeNode(toDel);
                    listSize = getSizeSafe();
                    if (listSize == 0) {
                        selectedIndex = 0;
                        startIndex = 0;
                        selected = nullptr;
                    } else {
                        if (selectedIndex >= listSize) {
                            selectedIndex = listSize - 1;
                        }
                        selected = getNodeByIndex(selectedIndex);
                    }
                }
            }
        } break;
        case 'W':
        case 'w': {
            if (listSize > 0 && selected && selected->tree) {
                selected->tree->mirrorTree();
            }
        } break;
        case 'Q':
        case 'q':
            running = false;
            break;
        default:
            std::cout << "[Gecersiz komut!]\n";
            break;
        }

        // Sayfa başlama noktasını güncelle
        if (listSize > 0) {
            if (selectedIndex < startIndex) {
                startIndex = (selectedIndex / pageSize) * pageSize;
            } else if (selectedIndex >= startIndex + pageSize) {
                startIndex = (selectedIndex / pageSize) * pageSize;
            }
            selected = getNodeByIndex(selectedIndex);
        }
    }

    std::cout << "Program kapaniyor...\n";
    return 0;
}
