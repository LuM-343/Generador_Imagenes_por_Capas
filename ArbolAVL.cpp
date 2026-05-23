//Arbol AVL para guardar las capas a usar para crear las imagenes//
//Tambien el arbol AVL se utilizara para guardar a los usuarios//

#include <iostream>
#include <algorithm> // Para std::max
#include <string>

using namespace std;

// ==========================================
// 1. ESTRUCTURAS Y ÁRBOL AVL PARA CAPAS
// ==========================================

class LayerNode {
public:
    int id;
    string name; //Para agregar la MatrizDispersa
    LayerNode* left;
    LayerNode* right;
    int height;

    LayerNode(int id, string name) {
        this->id = id;
        this->name = name;
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
        if (node == nullptr)
            return new LayerNode(id, name);

        // Ordenamos numéricamente por ID
        if (id < node->id)
            node->left = insert(node->left, id, name);
        else if (id > node->id)
            node->right = insert(node->right, id, name);
        else 
            return node; // No se permiten IDs duplicados

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Balanceo
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

public:
    LayerAVLTree() { root = nullptr; }

    void insert(int id, string name) {
        root = insert(root, id, name);
    }

    void display() {
        inorden(root);
        cout << endl;
    }
};

// ==========================================
// 2. ESTRUCTURAS Y ÁRBOL AVL PARA USUARIOS
// ==========================================

class UserNode {
public:
    string username;
    UserNode* left;
    UserNode* right;
    int height;

    UserNode(string username) {
        this->username = username;
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
        if (node == nullptr)
            return new UserNode(username);

        // Ordenamos alfabéticamente por username (C++ permite comparar strings con < y >)
        if (username < node->username)
            node->left = insert(node->left, username);
        else if (username > node->username)
            node->right = insert(node->right, username);
        else 
            return node; // No se permiten usuarios duplicados

        node->height = 1 + max(height(node->left), height(node->right));
        int balance = getBalance(node);

        // Balanceo
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

public:
    UserAVLTree() { root = nullptr; }

    void insert(string username) {
        root = insert(root, username);
    }

    void display() {
        inorden(root);
        cout << endl;
    }
};

// ==========================================
// 3. FUNCIÓN PRINCIPAL DE PRUEBA
// ==========================================

int main() {
    // Prueba del Árbol de Capas
    cout << "--- ARBOL AVL DE CAPAS (Ordenado por ID) ---" << endl;
    LayerAVLTree treeLayers;
    
    // Insertamos desordenado para comprobar el balanceo
    treeLayers.insert(50, "Fondo");
    treeLayers.insert(30, "Montanas");
    treeLayers.insert(70, "Cielo");
    treeLayers.insert(20, "Sol");
    treeLayers.insert(40, "Nubes");
    
    treeLayers.display();

    // Prueba del Árbol de Usuarios
    cout << "--- ARBOL AVL DE USUARIOS (Ordenado Alfabeticamente) ---" << endl;
    UserAVLTree treeUsers;

    // Insertamos desordenado
    treeUsers.insert("maria_dev");
    treeUsers.insert("carlos_admin");
    treeUsers.insert("ana_root");
    treeUsers.insert("zac_user");
    treeUsers.insert("luis_gamer");

    treeUsers.display();

    return 0;
}