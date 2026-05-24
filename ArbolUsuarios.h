#pragma once
#include <iostream>
#include <algorithm>
#include <string>

using namespace std;

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