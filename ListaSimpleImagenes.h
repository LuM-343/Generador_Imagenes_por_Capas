#pragma once
#include <iostream>
#include <string>

using namespace std;

// Nodo para la lista simple de imágenes de un usuario
struct NodoImagenSimple {
    int idImagen;
    string nombreImagen;
    NodoImagenSimple* siguiente;

    NodoImagenSimple(int id, string nombre) {
        idImagen = id;
        nombreImagen = nombre;
        siguiente = nullptr;
    }
};

class ListaSimpleImagenes {
private:
    NodoImagenSimple* cabeza;

public:
    ListaSimpleImagenes() {
        cabeza = nullptr;
    }

    // Insertar al final
    void insertar(int id, string nombre) {
        NodoImagenSimple* nuevo = new NodoImagenSimple(id, nombre);
        if (cabeza == nullptr) {
            cabeza = nuevo;
        } else {
            NodoImagenSimple* temp = cabeza;
            while (temp->siguiente != nullptr) {
                temp = temp->siguiente;
            }
            temp->siguiente = nuevo;
        }
    }

    void mostrar() {
        if (cabeza == nullptr) {
            cout << "   (Este usuario no ha creado imagenes)" << endl;
            return;
        }
        NodoImagenSimple* temp = cabeza;
        while (temp != nullptr) {
            cout << "   -> [ID: " << temp->idImagen << "] " << temp->nombreImagen << endl;
            temp = temp->siguiente;
        }
    }
};