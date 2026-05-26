#pragma once
#include <iostream>
#include <algorithm>
#include <string>
#include <fstream>
#include <vector>
#include "MatrizDispersa.h" // Conectamos la matriz con la capa

using namespace std;

class LayerNode {
public:
    int id;
    string name;
    LayerMatrix* matriz; // Cada capa tiene su propia matriz de pixeles
    LayerNode* left;
    LayerNode* right;
    int height;

    LayerNode(int id, string name) {
        this->id = id;
        this->name = name;
        this->matriz = new LayerMatrix(); // Inicializamos la matriz vacia
        left = right = nullptr;
        height = 1;
    }
};

class LayerAVLTree {
private:
    LayerNode* root;

    int height(LayerNode* N) {
        if (N == nullptr) return 0;
        return N->height;
    }

    int getBalance(LayerNode* N) {
        if (N == nullptr) return 0;
        return height(N->left) - height(N->right);
    }

    LayerNode* rightRotate(LayerNode* y) {
        LayerNode* x = y->left;
        LayerNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        y->height = max(height(y->left), height(y->right)) + 1;
        x->height = max(height(x->left), height(x->right)) + 1;
        return x;
    }

    LayerNode* leftRotate(LayerNode* x) {
        LayerNode* y = x->right;
        LayerNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;
        return y;
    }

    LayerNode* insert(LayerNode* node, int id, string name) {
        if (node == nullptr) return new LayerNode(id, name);

        if (id < node->id) node->left = insert(node->left, id, name);
        else if (id > node->id) node->right = insert(node->right, id, name);
        else return node;

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        if (balance > 1 && id < node->left->id) return rightRotate(node);
        if (balance < -1 && id > node->right->id) return leftRotate(node);
        if (balance > 1 && id > node->left->id) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && id < node->right->id) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorden(LayerNode* node) {
        if (node != nullptr) {
            inorden(node->left);
            cout << "Capa ID: " << node->id << " | Nombre: " << node->name << endl;
            inorden(node->right);
        }
    }

    LayerNode* search(LayerNode* node, int id) {
        if (node == nullptr || node->id == id)
            return node;

        if (node->id < id)
            return search(node->right, id);

        return search(node->left, id);
    }

    void generarDOT(LayerNode* node, ofstream& archivo) {
        if (node != nullptr) {
            archivo << "    \"Capa_" << node->id << "\\n" << node->name << "\" [style=filled, fillcolor=lightgreen];\n";
            
            if (node->left != nullptr) {
                archivo << "    \"Capa_" << node->id << "\\n" << node->name << "\" -> \"Capa_" << node->left->id << "\\n" << node->left->name << "\";\n";
                generarDOT(node->left, archivo);
            }
            if (node->right != nullptr) {
                archivo << "    \"Capa_" << node->id << "\\n" << node->name << "\" -> \"Capa_" << node->right->id << "\\n" << node->right->name << "\";\n";
                generarDOT(node->right, archivo);
            }
        }
    }

public:
    LayerAVLTree() { root = nullptr; }

    void insert(int id, string name) {
        root = insert(root, id, name);
    }

    void display() {
        inorden(root);
        cout << endl;
    }

    LayerNode* search(int id) {
        return search(root, id);
    }

    void graficarArbol(string nombreArchivo) {
        if (root == nullptr) {
            cout << "[AVISO] El arbol de capas esta vacio." << endl;
            return;
        }

        ofstream archivo(nombreArchivo + ".dot");
        archivo << "digraph ArbolCapas {\n";
        archivo << "    node [shape=box];\n";
        generarDOT(root, archivo);
        archivo << "}\n";
        archivo.close();

        string comando = "dot -Tpng " + nombreArchivo + ".dot -o " + nombreArchivo + ".png";
        system(comando.c_str());
        cout << "[OK] Arbol de capas graficado como " << nombreArchivo << ".png" << endl;
    }

    // Función auxiliar para Inorden Limitado
    void inordenLimitado(LayerNode* node, vector<LayerNode*>& lista, int limite) {
        if (node == nullptr || (int)lista.size() >= limite) return;
        inordenLimitado(node->left, lista, limite);
        if ((int)lista.size() < limite) lista.push_back(node);
        inordenLimitado(node->right, lista, limite);
    }

    // Función auxiliar para Preorden Limitado
    void preordenLimitado(LayerNode* node, vector<LayerNode*>& lista, int limite) {
        if (node == nullptr || (int)lista.size() >= limite) return;
        lista.push_back(node);
        preordenLimitado(node->left, lista, limite);
        preordenLimitado(node->right, lista, limite);
    }

    // Función auxiliar para Postorden Limitado
    void postordenLimitado(LayerNode* node, vector<LayerNode*>& lista, int limite) {
        if (node == nullptr || (int)lista.size() >= limite) return;
        postordenLimitado(node->left, lista, limite);
        postordenLimitado(node->right, lista, limite);
        if ((int)lista.size() < limite) lista.push_back(node);
    }
    
    // Métodos para obtener los vectores
    vector<LayerNode*> obtenerCapas(int limite, string recorrido) {
        vector<LayerNode*> lista;
        if (recorrido == "inorden") inordenLimitado(root, lista, limite);
        else if (recorrido == "preorden") preordenLimitado(root, lista, limite);
        else if (recorrido == "postorden") postordenLimitado(root, lista, limite);
        return lista;
    }
};