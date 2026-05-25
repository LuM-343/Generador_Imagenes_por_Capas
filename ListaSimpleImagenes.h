#pragma once
#include <iostream>
#include <string>
#include <fstream>
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

    bool eliminar(int id) {
        if (cabeza == nullptr) return false;
        
        // Si es el primero
        if (cabeza->idImagen == id) {
            NodoImagenSimple* temp = cabeza;
            cabeza = cabeza->siguiente;
            delete temp;
            return true;
        }

        // Buscar en el resto de la lista
        NodoImagenSimple* actual = cabeza;
        while (actual->siguiente != nullptr && actual->siguiente->idImagen != id) {
            actual = actual->siguiente;
        }

        if (actual->siguiente != nullptr) {
            NodoImagenSimple* temp = actual->siguiente;
            actual->siguiente = actual->siguiente->siguiente;
            delete temp;
            return true;
        }
        return false;
    }

    // Método para graficar la lista simple de imágenes de un usuario
    void graficarLista(string username) {
        if (cabeza == nullptr) {
            cout << "[AVISO] El usuario '" << username << "' no tiene imagenes para graficar." << endl;
            return;
        }

        string nombreArchivo = "Imagenes_" + username;
        ofstream archivo(nombreArchivo + ".dot");
        
        archivo << "digraph ListaImagenesUsuario {\n";
        archivo << "  rankdir=LR;\n";
        archivo << "  node [shape=record, style=filled, fillcolor=plum1];\n";
        
        // Título del grafo
        archivo << "  labelloc=\"t\";\n";
        archivo << "  label=\"Imagenes creadas por: " << username << "\";\n";

        NodoImagenSimple* temp = cabeza;
        
        // Declarar nodos y conexiones
        while (temp != nullptr) {
            // El formato record permite mostrar ID y Nombre divididos por una línea (|)
            archivo << "  Img" << temp->idImagen << " [label=\"{ID: " << temp->idImagen << " | " << temp->nombreImagen << "}\"];\n";
            
            if (temp->siguiente != nullptr) {
                archivo << "  Img" << temp->idImagen << " -> Img" << temp->siguiente->idImagen << ";\n";
            }
            temp = temp->siguiente;
        }

        archivo << "}\n";
        archivo.close();

        // Ejecutar Graphviz
        string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
        system(comando.c_str());
        cout << "[OK] Lista de imagenes del usuario graficada como " << nombreArchivo << ".png" << endl;
    }
};