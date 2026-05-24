#include <iostream>
#include <string>
#include <vector>
#include "ArbolUsuarios.h"
#include "ListaImagenes.h"
#include "ArbolCapas.h"
#include "MatrizDispersa.h"
#include "CargaMasiva.h"
#include "GeneradorImagenes.h"

using namespace std;

// ==========================================
// ESTRUCTURAS GLOBALES DEL SISTEMA
// ==========================================
UserAVLTree sistemaUsuarios;
ListaCircularDobleImagenes galeriaGlobal;
LayerAVLTree arbolCapasGlobal;

// ==========================================
// PROTOTIPOS DE FUNCIONES (MENUS)
// ==========================================
void menuInicio();
void menuUsuario(UserNode* usuarioActual);
void menuEditor(Imagen* imagenActual);
void menuReportes();
void menuCargaMasiva();
void menuGeneracionHTML();

// ==========================================
// FUNCIoN PRINCIPAL
// ==========================================
int main() {
    menuInicio();
    return 0;
}

// ==========================================
// 1. MENÚ DE INICIO
// ==========================================
void menuInicio() {
    int opcion = 0;
    string username;

    while (opcion != 6) {
        cout << "\n=========================================" << endl;
        cout << "   EDITOR DE PIXEL ART - PROYECTO FINAL  " << endl;
        cout << "=========================================" << endl;
        cout << "1. Iniciar Sesion" << endl;
        cout << "2. Registrar nuevo usuario" << endl;
        cout << "3. Reportes del Estado de Memoria (Graphviz)" << endl;
        cout << "4. Carga Masiva de Datos" << endl;
        cout << "5. Generacion de Imagenes (HTML)" << endl;
        cout << "6. Salir del programa" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1:
                cout << "\nIngrese su nombre de usuario: ";
                cin >> username;
                if (UserNode* usuarioLogueado = sistemaUsuarios.search(username)) {
                    cout << "\n[OK] Bienvenido de nuevo, " << username << "!" << endl;
                    menuUsuario(usuarioLogueado);
                } else {
                    cout << "\n[ERROR] El usuario '" << username << "' no existe." << endl;
                }
                break;
            case 2:
                cout << "\nIngrese el nombre para el nuevo usuario: ";
                cin >> username;
                if (sistemaUsuarios.search(username)) {
                    cout << "\n[ERROR] El nombre de usuario ya esta en uso." << endl;
                } else {
                    sistemaUsuarios.insert(username);
                    cout << "\n[OK] Usuario '" << username << "' registrado exitosamente." << endl;
                }
                break;
            case 3:
                menuReportes();
                break;
            case 4:
                menuCargaMasiva();
                break;
            case 5:
                menuGeneracionHTML();
                break;
            case 6:
                cout << "\nGuardando cambios... ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida." << endl;
        }
    }
}

// ==========================================
// 2. MENÚ DE USUARIO (Mantenimiento de Imágenes)
// ==========================================
void menuUsuario(UserNode* usuarioActual) {
    int opcion = 0;

    while (opcion != 4) {
        cout << "\n-----------------------------------------" << endl;
        cout << "  MENU DE USUARIO: " << usuarioActual->username << endl;
        cout << "-----------------------------------------" << endl;
        cout << "1. Crear una nueva imagen" << endl;
        cout << "2. Seleccionar / Editar una imagen" << endl;
        cout << "3. Ver mis imagenes creadas (Historial)" << endl;
        cout << "4. Cerrar Sesion (Regresar)" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1: {
                int idImg; string nombreImg;
                cout << "\nIngrese un ID numerico para la nueva imagen: ";
                cin >> idImg;
                if (galeriaGlobal.buscar(idImg)) {
                    cout << "[ERROR] Ya existe una imagen con ese ID." << endl; break;
                }
                cout << "Ingrese el nombre de la imagen: ";
                cin.ignore(); getline(cin, nombreImg);

                galeriaGlobal.insertar(idImg, nombreImg);
                usuarioActual->imagenesCreadas->insertar(idImg, nombreImg);
                cout << "[OK] Imagen creada correctamente." << endl;
                break;
            }
            case 2: {
                int idBuscar; cout << "\nIngrese el ID de la imagen a editar: "; cin >> idBuscar;
                if (Imagen* imgEncontrada = galeriaGlobal.buscar(idBuscar)) {
                    cout << "\n[OK] Abriendo el editor para: " << imgEncontrada->nombre << endl;
                    menuEditor(imgEncontrada);
                } else {
                    cout << "\n[ERROR] Imagen no encontrada." << endl;
                }
                break;
            }
            case 3:
                cout << "\n--- HISTORIAL DE PROYECTOS ---" << endl;
                usuarioActual->imagenesCreadas->mostrar();
                break;
            case 4:
                cout << "\nCerrando sesion..." << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida." << endl;
        }
    }
}

// ==========================================
// 3. MENÚ DE EDITOR (Matriz Dispersa y Capas)
// ==========================================
void menuEditor(Imagen* imagenActual) {
    int opcion = 0;

    while (opcion != 4) {
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "  EDITOR: " << imagenActual->nombre << " (ID: " << imagenActual->id << ")" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "1. Agregar una capa al proyecto" << endl;
        cout << "2. Pintar un pixel en una capa" << endl;
        cout << "3. Ver listado de capas aplicadas" << endl;
        cout << "4. Salir del editor" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1: {
                int idCapa; cout << "\nIngrese el ID numerico para anadir capa: "; cin >> idCapa;
                if (arbolCapasGlobal.search(idCapa) == nullptr) {
                    cout << "[ERROR] Esta capa no existe en el arbol global. Creela primero." << endl; break;
                }
                imagenActual->capasUtilizadas->insertar(idCapa);
                cout << "[OK] Capa vinculada al proyecto." << endl;
                break;
            }
            case 2: {
                int idCapa, fila, col; string colorHex;
                cout << "\n¿En que ID de capa desea pintar?: "; cin >> idCapa;
                if (LayerNode* capa = arbolCapasGlobal.search(idCapa)) {
                    cout << "Ingrese Fila: "; cin >> fila;
                    cout << "Ingrese Columna: "; cin >> col;
                    cout << "Ingrese Color Hexadecimal (#FFFFFF): "; cin >> colorHex;
                    capa->matriz->insert(fila, col, colorHex);
                    cout << "[OK] Pixel actualizado." << endl;
                } else cout << "[ERROR] Capa no encontrada." << endl;
                break;
            }
            case 3:
                cout << "\nSecuencia de capas utilizadas: ";
                imagenActual->capasUtilizadas->mostrarSecuencia();
                cout << endl;
                break;
            case 4:
                cout << "\nSaliendo del editor..." << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion invalida." << endl;
        }
    }
}

// ==========================================
// 4. MENÚ DE REPORTES (Graphviz)
// ==========================================
void menuReportes() {
    int opcion = 0;

    while (opcion != 6) {
        cout << "\n--- REPORTES DE MEMORIA (GRAPHVIZ) ---" << endl;
        cout << "1. Arbol AVL de Usuarios" << endl;
        cout << "2. Lista Circular Doble de Imagenes" << endl;
        cout << "3. Arbol Global de Capas (ABB/AVL)" << endl;
        cout << "4. Matriz Dispersa de una Capa" << endl;
        cout << "5. Lista Simple de Imagenes de un Usuario" << endl;
        cout << "6. Regresar al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1:
                sistemaUsuarios.graficarArbol(); break;
            case 2:
                galeriaGlobal.graficarLista(); break;
            case 3:
                arbolCapasGlobal.graficarArbol("Arbol_Global_Capas"); break;
            case 4: {
                int idCapa; cout << "\nID de la capa a graficar (Matriz Dispersa): "; cin >> idCapa;
                if (LayerNode* capa = arbolCapasGlobal.search(idCapa)) {
                    capa->matriz->graficarMatriz("Matriz_Capa_" + to_string(idCapa));
                } else cout << "[ERROR] Capa no encontrada." << endl;
                break;
            }
            case 5: {
                string user; cout << "\nIngrese el nombre del usuario: "; cin >> user;
                if (UserNode* nodoUser = sistemaUsuarios.search(user)) {
                    nodoUser->imagenesCreadas->graficarLista(user);
                } else cout << "[ERROR] Usuario no encontrado." << endl;
                break;
            }
            case 6: break;
            default: cout << "\n[ERROR] Opcion no valida." << endl;
        }
    }
}

// ==========================================
// 5. MENÚ DE CARGA MASIVA
// ==========================================
void menuCargaMasiva() {
    int opcion = 0;
    string rutaArchivo;

    while (opcion != 4) {
        cout << "\n--- CARGA MASIVA DE DATOS ---" << endl;
        cout << "1. Cargar archivo de Capas (.cap)" << endl;
        cout << "2. Cargar archivo de Imagenes (.im)" << endl;
        cout << "3. Cargar archivo de Usuarios (.usr)" << endl;
        cout << "4. Regresar al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1:
                cout << "\nRuta del archivo (.cap): ";
                cin.ignore(); getline(cin, rutaArchivo);
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"') rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                CargaMasiva::cargarCapas(rutaArchivo, arbolCapasGlobal);
                break;
            case 2:
                cout << "\nRuta del archivo (.im): ";
                cin.ignore(); getline(cin, rutaArchivo);
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"') rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                CargaMasiva::cargarImagenes(rutaArchivo, galeriaGlobal);
                break;
            case 3:
                cout << "\nRuta del archivo (.usr): ";
                cin.ignore(); getline(cin, rutaArchivo);
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"') rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                CargaMasiva::cargarUsuarios(rutaArchivo, sistemaUsuarios, galeriaGlobal);
                break;
            case 4: break;
            default: cout << "\n[ERROR] Opcion no valida." << endl;
        }
    }
}

// ==========================================
// 6. MENÚ DE GENERACIoN HTML
// ==========================================
void menuGeneracionHTML() {
    int opcion = 0;

    while (opcion != 5) {
        cout << "\n--- GENERACION DE IMAGENES (HTML) ---" << endl;
        cout << "1. Por Recorrido Limitado (Arbol de Capas Global)" << endl;
        cout << "2. Por Lista de Imagenes" << endl;
        cout << "3. Por Capa Individual" << endl;
        cout << "4. Por Usuario" << endl;
        cout << "5. Regresar al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        
        if (!(cin >> opcion)) { cin.clear(); cin.ignore(10000, '\n'); continue; }

        switch (opcion) {
            case 1: {
                int n; string recorrido;
                cout << "Ingrese el numero de capas (n): "; cin >> n;
                cout << "Ingrese el tipo de recorrido (preorden, inorden, postorden): "; cin >> recorrido;
                vector<LayerNode*> capasObtenidas = arbolCapasGlobal.obtenerCapas(n, recorrido);
                GeneradorImagenes::exportarHTML("Arte_Recorrido_" + recorrido, capasObtenidas);
                break;
            }
            case 2: {
                int idImg; cout << "Ingrese el ID de la imagen a generar: "; cin >> idImg;
                if (Imagen* img = galeriaGlobal.buscar(idImg)) {
                    vector<LayerNode*> capasDeImagen;
                    NodoCapaSimple* tempCapaId = img->capasUtilizadas->getCabeza(); 
                    while(tempCapaId != nullptr) {
                        if (LayerNode* nodoReal = arbolCapasGlobal.search(tempCapaId->idCapa)) capasDeImagen.push_back(nodoReal);
                        tempCapaId = tempCapaId->siguiente;
                    }
                    GeneradorImagenes::exportarHTML(img->nombre, capasDeImagen);
                } else cout << "[ERROR] Imagen no encontrada." << endl;
                break;
            }
            case 3: {
                int idCapa; cout << "Ingrese el ID de la capa aislada a generar: "; cin >> idCapa;
                if (LayerNode* capa = arbolCapasGlobal.search(idCapa)) {
                    vector<LayerNode*> unicaCapa = { capa };
                    GeneradorImagenes::exportarHTML("CapaAislada_" + to_string(idCapa), unicaCapa);
                } else cout << "[ERROR] Capa no encontrada." << endl;
                break;
            }
            case 4: {
                string username; cout << "Ingrese el nombre del usuario: "; cin >> username;
                if (UserNode* user = sistemaUsuarios.search(username)) {
                    user->imagenesCreadas->mostrar();
                    cout << "\nSeleccione el ID de la imagen a generar: ";
                    int idImg; cin >> idImg;
                    if (Imagen* img = galeriaGlobal.buscar(idImg)) {
                        vector<LayerNode*> capasDeImagen;
                        NodoCapaSimple* tempCapaId = img->capasUtilizadas->getCabeza(); 
                        while(tempCapaId != nullptr) {
                            if (LayerNode* nodoReal = arbolCapasGlobal.search(tempCapaId->idCapa)) capasDeImagen.push_back(nodoReal);
                            tempCapaId = tempCapaId->siguiente;
                        }
                        GeneradorImagenes::exportarHTML("User_" + username + "_" + img->nombre, capasDeImagen);
                    } else cout << "[ERROR] La imagen no existe." << endl;
                } else cout << "[ERROR] Usuario no encontrado." << endl;
                break;
            }
            case 5: break;
            default: cout << "\n[ERROR] Opcion invalida." << endl;
        }
    }
}