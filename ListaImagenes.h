#pragma once
#include <iostream>
#include <string>
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
};