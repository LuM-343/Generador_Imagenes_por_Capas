#pragma once
#include <iostream>

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
};