//Matriz dispersa en la que se guardaran las capas para generar imagenes//
#pragma once
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

struct Node {
    int row;
    int col;
    string color;
    Node* right;
    Node* down;

    Node(int r, int c, string clr) : row(r), col(c), color(clr), right(nullptr), down(nullptr) {}
};

struct Header {
    int index;
    Header* next;
    Node* firstNode;

    Header(int index) : index(index), next(nullptr), firstNode(nullptr) {}
};

class LayerMatrix {
private:
    Header* rowHeaderRoot;
    Header* colHeaderRoot;

    Header* getOrCreateHeader(Header* &root, int idx) {
        if (!root || idx < root->index) {
            Header* newHeader = new Header(idx);
            newHeader->next = root;
            root = newHeader;
            return root;
        }

        Header* aux = root;
        while(aux->next && aux->next->index <= idx) {
            aux = aux->next;
        }

        if(aux->index == idx) return aux;

        Header *newHeader = new Header(idx);
        newHeader->next = aux->next;
        aux->next = newHeader;
        return newHeader;
    }

public:
    LayerMatrix() : rowHeaderRoot(nullptr), colHeaderRoot(nullptr) {}

    void insert(int r, int c, string clr) {
        if (clr == "" || clr == "#ffffff" || clr == "#FFFFFF") {
            remove(r, c);
            return;
        }

        Header* rh = getOrCreateHeader(rowHeaderRoot, r);
        Header* ch = getOrCreateHeader(colHeaderRoot, c);

        Node* existing = search(r, c);
        if (existing) {
            existing->color = clr;
            return;
        }
        
        Node* newNode = new Node(r, c, clr);
        
        if(!rh->firstNode || rh->firstNode->col > c) {
            newNode->right = rh->firstNode;
            rh->firstNode = newNode;
        } else {
            Node* temp = rh->firstNode;
            while (temp->right && temp->right->col < c) {
                temp = temp->right;
            }
            newNode->right = temp->right;
            temp->right = newNode;
        }
        
        if(!ch->firstNode || ch->firstNode->row > r) {
            newNode->down = ch->firstNode;
            ch->firstNode = newNode;
        } else {
            Node* temp = ch->firstNode;
            while (temp->down && temp->down->row < r) {
                temp = temp->down;
            }
            newNode->down = temp->down;
            temp->down = newNode;
        }
    } 

    Node* search(int r, int c) {
        Header* currRow = rowHeaderRoot;
        while (currRow && currRow->index < r) currRow = currRow->next;
        if (currRow && currRow->index == r) {
            Node* temp = currRow->firstNode;
            while (temp && temp->col < c) temp = temp->right;
            if (temp && temp->col == c) return temp;
        }
        return nullptr;
    }

    void remove(int r, int c) {
        Header* rh = rowHeaderRoot;
        while (rh && rh->index < r) rh = rh->next;
        Header* ch = colHeaderRoot;
        while (ch && ch->index < c) ch = ch->next;

        if (!rh || !ch) return;

        Node* target = nullptr;
        Node* prev = nullptr;
        Node* curr = rh->firstNode;

        while (curr && curr->col < c) {
            prev = curr;
            curr = curr->right;
        }
        if (!curr || curr->col != c) return;

        target = curr;
        if (!prev) rh->firstNode = target->right;
        else prev->right = target->right;

        prev = nullptr;
        curr = ch->firstNode;

        while (curr && curr->row < r) {
            prev = curr;
            curr = curr->down;
        }
        if (!prev) ch->firstNode = target->down;
        else prev->down = target->down;

        delete target;
    }

    void display(int maxR, int maxC) {
        for (int i = 0; i <= maxR; i++) {
            for (int j = 0; j <= maxC; j++) {
                Node* n = search(i, j);
                if (n) cout << setw(8) << n->color << " ";
                else cout << setw(8) << "---" << " ";
            }
            cout << endl << endl;
        }
    }
};