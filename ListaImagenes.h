#pragma once
#include <iostream>
#include <string>
#include "ArbolCapas.h" // Conectamos el arbol de capas con la imagen

using namespace std;

struct Imagen {
    int id;
    string nombre;
    LayerAVLTree* arbolCapas; // Cada imagen tiene su propio arbol de capas
    Imagen* siguiente;
    Imagen* anterior;

    Imagen(int _id, string _nombre) {
        id = _id;
        nombre = _nombre;
        arbolCapas = new LayerAVLTree(); // Inicializamos el arbol vacio
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
};