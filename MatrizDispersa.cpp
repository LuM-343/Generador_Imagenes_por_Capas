//Matriz dispersa en la que se guardaran las capas para generar imagenes//
#include <iostream>
#include <iomanip>
#include <string>

using namespace std;

// Estructura del Nodo adaptada para guardar colores en Hexadecimal
struct Node {
    int row;
    int col;
    string color; // Almacena el color en formato Hexadecimal (ej. "#e74c3c")
    Node* right;  // Puntero hacia la derecha
    Node* down;   // Puntero hacia abajo

    Node(int r, int c, string clr) : row(r), col(c), color(clr), right(nullptr), down(nullptr) {}
};

// Estructura de las cabeceras (filas y columnas)
struct Header {
    int index;
    Header* next;
    Node* firstNode; // Primer nodo siguiente de informacion

    Header(int index) : index(index), next(nullptr), firstNode(nullptr) {}
};

class LayerMatrix {
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

    // Inserción de un nuevo píxel (nodo) en la capa
    void insert(int r, int c, string clr) {
        // Si el color está vacío o es blanco puro, lo tratamos como espacio vacío
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
        
        // Enlazar en Fila
        if(!rh->firstNode || rh->firstNode->col > c) {
            newNode->right = rh->firstNode;
            rh->firstNode = newNode;
        } else {
            Node* temp = rh->firstNode;
            while (temp->right && temp->right->col < c) { // Corregido a temp->right->col < c
                temp = temp->right;
            }
            newNode->right = temp->right;
            temp->right = newNode;
        }
        
        // Enlazar en Columna
        if(!ch->firstNode || ch->firstNode->row > r) { // Corregido a ch->firstNode->row > r
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

        cout << "Pixel insertado [" << newNode->row << "][" << newNode->col << "] = " << newNode->color << endl;
    } 

    // Búsqueda de un píxel en una coordenada específica
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

    // Eliminación de un píxel
    void remove(int r, int c) {
        Header* rh = rowHeaderRoot;
        while (rh && rh->index < r) {
            rh = rh->next;
        }
        Header* ch = colHeaderRoot;
        while (ch && ch->index < c) { // Corregido a ch->index < c
            ch = ch->next;
        }

        if (!rh || !ch) return;

        // Desvinculación de filas
        Node* target = nullptr;
        Node* prev = nullptr;
        Node* curr = rh->firstNode;

        while (curr && curr->col < c) {
            prev = curr;
            curr = curr->right;
        }
        if (!curr || curr->col != c) return;

        target = curr;
        if (!prev) {
            rh->firstNode = target->right;
        } else {
            prev->right = target->right;
        }

        // Desvinculación de columnas
        prev = nullptr;
        curr = ch->firstNode;

        while (curr && curr->row < r) {
            prev = curr;
            curr = curr->down;
        }
        if (!prev) {
            ch->firstNode = target->down;
        } else {
            prev->down = target->down;
        }

        delete target;
        cout << "Pixel eliminado: [" << r << "][" << c << "]" << endl;
    }

    // Visualización en consola para pruebas (muestra --- si es blanco/vacío)
    void display(int maxR, int maxC) {
        cout << "\nVisualizacion de capa (Matriz Dispersa)\n" << endl;
        for (int i = 0; i <= maxR; i++) {
            for (int j = 0; j <= maxC; j++) {
                Node* n = search(i, j);
                if (n) {
                    cout << setw(8) << n->color << " ";
                } else {
                    cout << setw(8) << "---" << " ";
                }
            }
            cout << endl << endl;
        }
    }
};

int main() {
    LayerMatrix layer;

    // Pruebas insertando colores hexadecimales
    layer.insert(1, 1, "#ff0000"); // Rojo
    layer.insert(1, 3, "#00ff00"); // Verde
    layer.insert(4, 4, "#0000ff"); // Azul
    layer.insert(0, 0, "#000000"); // Negro

    layer.display(5, 5);

    // Simulando borrar un pixel insertando blanco o vacío
    layer.insert(1, 1, "");

    layer.display(5, 5);
    
    return 0;
}