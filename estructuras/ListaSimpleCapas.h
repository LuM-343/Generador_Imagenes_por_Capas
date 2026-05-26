#pragma once
#include <iostream>
#include <fstream>

using namespace std;

// Nodo para la lista simple de capas
struct NodoCapaSimple {
    int idCapa;
    NodoCapaSimple* siguiente;

    NodoCapaSimple(int id) {
        idCapa = id;
        siguiente = nullptr;
    }
};

class ListaSimpleCapas {
private:
    NodoCapaSimple* cabeza;

public:
    ListaSimpleCapas() {
        cabeza = nullptr;
    }

    NodoCapaSimple* getCabeza() {
        return cabeza;
    }
    // Insertar al final de la lista simple
    void insertar(int idCapa) {
        NodoCapaSimple* nuevo = new NodoCapaSimple(idCapa);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoCapaSimple* temp = cabeza;
            while (temp->siguiente != nullptr) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevo;
        }
    }

    // Muestra las capas en forma de secuencia (ej: 1 -> 2 -> 5)
    void mostrarSecuencia() {
        if (cabeza == nullptr) {
            cout << "(Ninguna capa en uso)";
            return;
        }
        NodoCapaSimple* temp = cabeza;
        while (temp != nullptr) {
            cout << temp->idCapa;
            if (temp->siguiente != nullptr) cout << " -> ";
            temp = temp->siguiente;
        }
    }

    // Método para graficar la lista simple de capas
    void graficarLista(string nombreArchivo) {
        if (cabeza == nullptr) {
            cout << "[AVISO] Esta lista de capas esta vacia." << endl;
            return;
        }

        ofstream archivo(nombreArchivo + ".dot");
        archivo << "digraph ListaSimpleCapas {\n";
        archivo << "  rankdir=LR;\n"; // De izquierda a derecha
        archivo << "  node [shape=box, style=filled, fillcolor=lightcyan];\n";

        NodoCapaSimple* temp = cabeza;
        
        // Declarar nodos y conexiones
        while (temp != nullptr) {
            archivo << "  Capa" << temp->idCapa << " [label=\"Capa " << temp->idCapa << "\"];\n";
            
            if (temp->siguiente != nullptr) {
                archivo << "  Capa" << temp->idCapa << " -> Capa" << temp->siguiente->idCapa << ";\n";
            }
            temp = temp->siguiente;
        }

        archivo << "}\n";
        archivo.close();

        // Ejecutar Graphviz
        string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
        system(comando.c_str());
        cout << "[OK] Lista simple de capas graficada como " << nombreArchivo << ".png" << endl;
    }
};