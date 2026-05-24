#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include "ArbolCapas.h" 
#include "ListaSimpleCapas.h" 

using namespace std;

struct Imagen {
    int id;
    string nombre;
    LayerAVLTree* arbolCapas; 
    ListaSimpleCapas* capasUtilizadas; 
    Imagen* siguiente;
    Imagen* anterior;

    Imagen(int _id, string _nombre) {
        id = _id;
        nombre = _nombre;
        arbolCapas = new LayerAVLTree(); 
        capasUtilizadas = new ListaSimpleCapas(); // <-- Inicializa la lista
        siguiente = NULL;
        anterior = NULL;
    }
};

class ListaCircularDobleImagenes {
private:
    Imagen* cabeza = NULL;

public:
    void insertar(int id, string nombre) {
        Imagen* nuevo = new Imagen(id, nombre);

        if (cabeza == NULL) {
            cabeza = nuevo;
            cabeza->siguiente = cabeza;
            cabeza->anterior = cabeza;
        } else {
            Imagen* ultimo = cabeza->anterior;
            ultimo->siguiente = nuevo;
            nuevo->anterior = ultimo;
            nuevo->siguiente = cabeza;
            cabeza->anterior = nuevo;
        }
        cout << "Imagen agregada: " << nombre << endl;
    }

    void mostrar() {
        if (cabeza == NULL) {
            cout << "La lista de imagenes esta vacia." << endl;
            return;
        }

        Imagen* temp = cabeza;
        cout << "\n--- GALERIA DE IMAGENES ---" << endl;

        do {
            cout << "ID: " << temp->id << " | Nombre: " << temp->nombre << endl;
            temp = temp->siguiente;           
        } while (temp != cabeza);
    }

    Imagen* buscar(int id) {
        if (cabeza == NULL) {
            return NULL; // La lista está vacía
        }

        Imagen* temp = cabeza;
        do {
            if (temp->id == id) {
                return temp; // ¡Imagen encontrada!
            }
            temp = temp->siguiente;
        } while (temp != cabeza);

        return NULL; // Dio la vuelta completa y no la encontró
    }

    // Método para graficar (EN ListaCircularDobleImagenes)
    void graficarLista() {
        if (cabeza == NULL) {
            cout << "[AVISO] La lista de imagenes esta vacia." << endl;
            return;
        }

        ofstream archivo("lista_imagenes.dot");
        archivo << "digraph ListaImagenes {\n";
        archivo << "  rankdir=LR;\n"; // Alineación de izquierda a derecha
        
        // CAMBIO CLAVE: Usamos 'none' para habilitar las tablas HTML
        archivo << "  node [shape=none];\n"; 

        Imagen* temp = cabeza;
        
        // 1. Declarar los nodos usando sintaxis HTML de Graphviz
        do {
            archivo << "  Img" << temp->id << " [label=<\n";
            archivo << "    <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" BGCOLOR=\"lightyellow\">\n";
            archivo << "      <TR><TD>ID: " << temp->id << "</TD><TD>" << temp->nombre << "</TD></TR>\n";
            archivo << "    </TABLE>>];\n";
            temp = temp->siguiente;
        } while (temp != cabeza);

        // 2. Conectar la lista doblemente enlazada circular
        temp = cabeza;
        do {
            archivo << "  Img" << temp->id << " -> Img" << temp->siguiente->id << ";\n";
            archivo << "  Img" << temp->id << " -> Img" << temp->anterior->id << ";\n";
            
            temp = temp->siguiente;
        } while (temp != cabeza);

        archivo << "}\n";
        archivo.close();

        system("dot -Tpng lista_imagenes.dot -o Reporte_ListaImagenes.png");
        cout << "[OK] Lista circular de imagenes graficada sin errores." << endl;
    }
};