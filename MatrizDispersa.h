//Matriz dispersa en la que se guardaran las capas para generar imagenes//
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>

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

    void exportarImagenHTML(string nombreArchivo, int maxFilas, int maxCols) {
        ofstream archivo(nombreArchivo + ".html");

        // Escribimos la estructura base del HTML y CSS
        archivo << "<!DOCTYPE html>\n<html>\n<head>\n<style>\n";
        archivo << "  .lienzo { display: grid; grid-template-columns: repeat(" << maxCols << ", 20px); gap: 1px; background-color: #ccc; width: max-content; border: 2px solid black; }\n";
        archivo << "  .pixel { width: 20px; height: 20px; background-color: #ffffff; }\n";
        archivo << "</style>\n</head>\n<body>\n";
        archivo << "<h2>Imagen Exportada: " << nombreArchivo << "</h2>\n";
        archivo << "<div class='lienzo'>\n";

        // Recorremos la matriz para pintar cada celda
        for (int i = 0; i < maxFilas; i++) {
            for (int j = 0; j < maxCols; j++) {
                Node* pixel = search(i, j);
                if (pixel != nullptr) {
                    // Si existe el pixel, usamos su color
                    archivo << "  <div class='pixel' style='background-color: " << pixel->color << ";'></div>\n";
                } else {
                    // Si no existe, lo dejamos en blanco (transparente)
                    archivo << "  <div class='pixel'></div>\n";
                }
            }
        }

        archivo << "</div>\n</body>\n</html>";
        archivo.close();

        cout << "[OK] Imagen exportada exitosamente como '" << nombreArchivo << ".html'." << endl;
        cout << "Puedes abrir este archivo en Google Chrome o Edge para ver tu dibujo." << endl;
    }
    
    // Método para graficar el estado de memoria de la Matriz Dispersa
    void graficarMatriz(string nombreArchivo) {
        ofstream archivo(nombreArchivo + ".dot");
        archivo << "digraph MatrizDispersa {\n";
        archivo << "  node [shape=box, style=filled, fillcolor=white];\n";
        archivo << "  rankdir=TB;\n";
        
        archivo << "  Raiz [label=\"Matriz\", group=0, fillcolor=black, fontcolor=white];\n";

        // 1. Graficar Cabeceras de Columnas
        Header* tempCol = colHeaderRoot;
        if (tempCol != nullptr) {
            archivo << "  Raiz -> Col" << tempCol->index << ";\n";
            archivo << "  { rank=same; Raiz; ";
            while (tempCol != nullptr) {
                archivo << "Col" << tempCol->index << " [label=\"C" << tempCol->index << "\", group=" << tempCol->index + 1 << ", fillcolor=lightgrey]; ";
                if (tempCol->next != nullptr) {
                    archivo << "Col" << tempCol->index << " -> Col" << tempCol->next->index << " [dir=both]; ";
                }
                tempCol = tempCol->next;
            }
            archivo << "}\n";
        }

        // 2. Graficar Cabeceras de Filas y Nodos Internos
        Header* tempRow = rowHeaderRoot;
        if (tempRow != nullptr) {
            archivo << "  Raiz -> Fila" << tempRow->index << ";\n";
            while (tempRow != nullptr) {
                // Nodo Cabecera de Fila
                archivo << "  Fila" << tempRow->index << " [label=\"F" << tempRow->index << "\", group=0, fillcolor=lightgrey];\n";
                if (tempRow->next != nullptr) {
                    archivo << "  Fila" << tempRow->index << " -> Fila" << tempRow->next->index << " [dir=both];\n";
                }

                // Nodos Internos de la Fila
                archivo << "  { rank=same; Fila" << tempRow->index << "; ";
                Node* tempNode = tempRow->firstNode;
                string nodoAnterior = "Fila" + to_string(tempRow->index);
                
                while (tempNode != nullptr) {
                    string nombreNodo = "N_F" + to_string(tempNode->row) + "_C" + to_string(tempNode->col);
                    // El label muestra el color Hexadecimal
                    archivo << nombreNodo << " [label=\"" << tempNode->color << "\", group=" << tempNode->col + 1 << "]; ";
                    archivo << nodoAnterior << " -> " << nombreNodo << " [dir=both]; ";
                    nodoAnterior = nombreNodo;
                    tempNode = tempNode->right;
                }
                archivo << "}\n";
                tempRow = tempRow->next;
            }
        }

        // 3. Conexiones Verticales (Columnas hacia abajo)
        tempCol = colHeaderRoot;
        while (tempCol != nullptr) {
            Node* tempNode = tempCol->firstNode;
            string nodoAnterior = "Col" + to_string(tempCol->index);
            while (tempNode != nullptr) {
                string nombreNodo = "N_F" + to_string(tempNode->row) + "_C" + to_string(tempNode->col);
                archivo << "  " << nodoAnterior << " -> " << nombreNodo << " [dir=both];\n";
                nodoAnterior = nombreNodo;
                tempNode = tempNode->down;
            }
            tempCol = tempCol->next;
        }

        archivo << "}\n";
        archivo.close();

        // Ejecutar Graphviz
        string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
        system(comando.c_str());
        cout << "[OK] Matriz graficada como " << nombreArchivo << ".png" << endl;
    }
};