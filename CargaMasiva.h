#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "ArbolUsuarios.h"
#include "ListaImagenes.h"
#include "ArbolCapas.h"

using namespace std;

class CargaMasiva {
public:
    // ==========================================
    // 1. CARGA MASIVA DE CAPAS (.cap)
    // ==========================================
    static void cargarCapas(string rutaArchivo, LayerAVLTree& arbolCapasGlobal) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cout << "[ERROR] No se pudo abrir el archivo de capas: " << rutaArchivo << endl;
            return;
        }

        string linea;
        int idCapaActual = -1;
        LayerNode* nodoCapaActual = nullptr;

        while (getline(archivo, linea)) {
            // Limpiar espacios en blanco al inicio
            size_t inicio = linea.find_first_not_of(" \t\r\n");
            if (inicio == string::npos) continue; // Línea vacía
            linea = linea.substr(inicio);

            if (linea.find("{") != string::npos) {
                // Extraer el ID de la capa
                string idStr = linea.substr(0, linea.find("{"));
                idCapaActual = stoi(idStr);
                
                // Insertamos la nueva capa en el árbol general (asumimos un nombre por defecto)
                arbolCapasGlobal.insert(idCapaActual, "Capa_Masiva_" + to_string(idCapaActual));
                nodoCapaActual = arbolCapasGlobal.search(idCapaActual);
                
            } else if (linea.find("}") != string::npos) {
                // Fin de la capa actual
                idCapaActual = -1;
                nodoCapaActual = nullptr;
                
            } else if (linea.find(";") != string::npos && nodoCapaActual != nullptr) {
                // Leer los pixeles: fila, columna, color;
                // Reemplazamos las comas y el punto y coma con espacios para usar stringstream
                for (char& c : linea) {
                    if (c == ',' || c == ';') c = ' ';
                }
                
                stringstream ss(linea);
                int fila, col;
                string colorHex;
                
                if (ss >> fila >> col >> colorHex) {
                    // Insertamos el pixel en la matriz dispersa de la capa actual
                    nodoCapaActual->matriz->insert(fila, col, colorHex);
                }
            }
        }
        archivo.close();
        cout << "[OK] Carga masiva de CAPAS completada." << endl;
    }

    // ==========================================
    // 2. CARGA MASIVA DE IMAGENES (.im)
    // ==========================================
    static void cargarImagenes(string rutaArchivo, ListaCircularDobleImagenes& galeriaGlobal) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cout << "[ERROR] No se pudo abrir el archivo de imagenes: " << rutaArchivo << endl;
            return;
        }

        string linea;
        while (getline(archivo, linea)) {
            size_t posLlaveAbre = linea.find("{");
            size_t posLlaveCierra = linea.find("}");
            
            if (posLlaveAbre != string::npos && posLlaveCierra != string::npos) {
                // Extraer ID de la imagen
                string idImgStr = linea.substr(0, posLlaveAbre);
                int idImagen = stoi(idImgStr);
                
                // Insertar en la Lista Circular Doble
                galeriaGlobal.insertar(idImagen, "Imagen_Masiva_" + to_string(idImagen));
                Imagen* imgInsertada = galeriaGlobal.buscar(idImagen);

                // Extraer las capas separadas por coma
                string capasStr = linea.substr(posLlaveAbre + 1, posLlaveCierra - posLlaveAbre - 1);
                
                if (!capasStr.empty() && imgInsertada != nullptr) {
                    stringstream ss(capasStr);
                    string idCapaItem;
                    // Separar por comas
                    while (getline(ss, idCapaItem, ',')) {
                        int idCapa = stoi(idCapaItem);
                        imgInsertada->capasUtilizadas->insertar(idCapa);
                    }
                }
                // Nota: Si capasStr está vacío, la imagen no tiene capas.
                // Al generarse, el sistema deberá interpretarlo como un píxel negro.
            }
        }
        archivo.close();
        cout << "[OK] Carga masiva de IMAGENES completada." << endl;
    }

    // ==========================================
    // 3. CARGA MASIVA DE USUARIOS (.usr)
    // ==========================================
    static void cargarUsuarios(string rutaArchivo, UserAVLTree& sistemaUsuarios, ListaCircularDobleImagenes& galeriaGlobal) {
        ifstream archivo(rutaArchivo);
        if (!archivo.is_open()) {
            cout << "[ERROR] No se pudo abrir el archivo de usuarios: " << rutaArchivo << endl;
            return;
        }

        string contenido((istreambuf_iterator<char>(archivo)), istreambuf_iterator<char>());
        archivo.close();

        // Procesar por punto y coma (;)
        stringstream ssUsuarios(contenido);
        string bloqueUsuario;
        
        while (getline(ssUsuarios, bloqueUsuario, ';')) {
            size_t posDosPuntos = bloqueUsuario.find(":");
            if (posDosPuntos != string::npos) {
                // Limpiar posibles espacios en blanco
                size_t inicioUser = bloqueUsuario.find_first_not_of(" \n\r\t");
                if(inicioUser == string::npos) continue;
                
                string username = bloqueUsuario.substr(inicioUser, posDosPuntos - inicioUser);
                string imagenesStr = bloqueUsuario.substr(posDosPuntos + 1);
                
                // Insertar usuario
                sistemaUsuarios.insert(username);
                UserNode* nodoUser = sistemaUsuarios.search(username);
                
                // Si tiene imágenes asociadas, agregarlas a su lista simple
                if (!imagenesStr.empty() && nodoUser != nullptr) {
                    stringstream ssImg(imagenesStr);
                    string idImgItem;
                    while (getline(ssImg, idImgItem, ',')) {
                        int idImg = stoi(idImgItem);
                        // Buscar la imagen en la galería para obtener su nombre (opcional pero recomendado)
                        Imagen* imgRef = galeriaGlobal.buscar(idImg);
                        string nombreImg = (imgRef != nullptr) ? imgRef->nombre : "Imagen_Desconocida";
                        
                        nodoUser->imagenesCreadas->insertar(idImg, nombreImg);
                    }
                }
            }
        }
        cout << "[OK] Carga masiva de USUARIOS completada." << endl;
    }
};