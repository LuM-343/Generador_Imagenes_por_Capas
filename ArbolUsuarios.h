#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include "ListaSimpleImagenes.h" 
#include <fstream>

using namespace std;

class UserNode {
public:
    string username;
    ListaSimpleImagenes* imagenesCreadas; 
    UserNode* left;
    UserNode* right;
    int height;

    UserNode(string username) {
        this->username = username;
        this->imagenesCreadas = new ListaSimpleImagenes();
        left = right = nullptr;
        height = 1;
    }
};

class UserAVLTree {
private:
    UserNode* root;

    int height(UserNode* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int getBalance(UserNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    UserNode* rightRotate(UserNode* y) {
        UserNode* x = y->left;
        UserNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    UserNode* leftRotate(UserNode* x) {
        UserNode* y = x->right;
        UserNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    UserNode* insert(UserNode* node, string username) {
        if (node == nullptr) return new UserNode(username);

        if (username < node->username) node->left = insert(node->left, username);
        else if (username > node->username) node->right = insert(node->right, username);
        else return node; 

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && username < node->left->username) return rightRotate(node);
        if (balance < -1 && username > node->right->username) return leftRotate(node);
        if (balance > 1 && username > node->left->username) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && username < node->right->username) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorden(UserNode* node) {
        if (node != nullptr) {
            inorden(node->left);
            cout << "Usuario: " << node->username << endl;
            inorden(node->right);
        }
    }

    UserNode* search(UserNode* node, string username) {
        // Caso base: raíz nula o la llave está presente en la raíz
        if (node == nullptr || node->username == username)
            return node;

        // La llave es mayor que la llave de la raíz
        if (node->username < username)
            return search(node->right, username);

        // La llave es menor que la llave de la raíz
        return search(node->left, username);
    }

    void generarDOT(UserNode* node, ofstream& archivo) {
        if (node != nullptr) {
            // Escribir relación con el hijo izquierdo
            if (node->left != nullptr) {
                archivo << "    \"" << node->username << "\" -> \"" << node->left->username << "\";\n";
                generarDOT(node->left, archivo);
            }
            // Escribir relación con el hijo derecho
            if (node->right != nullptr) {
                archivo << "    \"" << node->username << "\" -> \"" << node->right->username << "\";\n";
                generarDOT(node->right, archivo);
            }
        }
    }

public:
    UserAVLTree() { root = nullptr; }

    void insert(string username) {
        root = insert(root, username);
    }

    UserNode* search(string username) {
        return search(root, username);
    }

    void display() {
        inorden(root);
        cout << endl;
    }

    void graficarArbol() {
        if (root == nullptr) {
            cout << "[AVISO] El arbol de usuarios esta vacio." << endl;
            return;
        }

        // 1. Crear el archivo .dot
        ofstream archivo("arbol_usuarios.dot");
        archivo << "digraph G {\n";
        archivo << "    node [shape=record, style=filled, fillcolor=lightblue];\n";
        
        // 2. Llamar a la función recursiva
        generarDOT(root, archivo);
        
        archivo << "}\n";
        archivo.close();

        // 3. Ejecutar comando de Graphviz en la terminal
        // Nota: Debes tener Graphviz instalado en tu PC y agregado al PATH
        system("dot -Tpng arbol_usuarios.dot -o Reporte_Usuarios.png");
        
        cout << "[OK] Reporte generado exitosamente como 'Reporte_Usuarios.png'." << endl;
    }
};