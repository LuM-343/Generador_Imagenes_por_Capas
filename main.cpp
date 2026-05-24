#include <iostream>
#include <string>
#include "ArbolUsuarios.h"
#include "ListaImagenes.h"
#include "ArbolCapas.h"
#include "MatrizDispersa.h"

using namespace std;

// Declaración de estructuras globales necesarias para todo el sistema
UserAVLTree sistemaUsuarios;
ListaCircularDobleImagenes galeriaGlobal;

// Prototipos de funciones de los menús
void menuInicio();
void menuUsuario(UserNode* usuarioActual);
void menuEditor(Imagen* imagenActual);

int main() {
    // Insertamos un par de usuarios y datos de prueba para no empezar desde cero
    sistemaUsuarios.insert("carlos_art");
    sistemaUsuarios.insert("ana_pixel");
    
    // Iniciar el sistema
    menuInicio();
    return 0;
}

// =========================================================================
// LEVEL 1: MENÚ DE INICIO (Login / Registro)
// =========================================================================
void menuInicio() {
    int opcion = 0;
    string username;

    while (opcion != 3) {
        cout << "\n=========================================" << endl;
        cout << "         EDITOR DE PIXEL ART - INICIO    " << endl;
        cout << "=========================================" << endl;
        cout << "1. Iniciar Sesion" << endl;
        cout << "2. Registrar nuevo usuario" << endl;
        cout << "3. Salir del programa" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "\nIngrese su nombre de usuario: ";
                cin >> username;
                
                // Buscamos al usuario en el árbol AVL
                UserNode* usuarioLogueado = sistemaUsuarios.search(username);
                if (usuarioLogueado != nullptr) {
                    cout << "\n[OK] Bienvenido de nuevo, " << username << "!" << endl;
                    menuUsuario(usuarioLogueado); // Pasar al siguiente nivel de menú
                } else {
                    cout << "\n[ERROR] El usuario '" << username << "' no existe." << endl;
                }
                break;
            }
            case 2:
                cout << "\nIngrese el nombre para el nuevo usuario: ";
                cin >> username;
                // Verificamos primero si ya existe para evitar duplicados
                if (sistemaUsuarios.search(username) != nullptr) {
                    cout << "\n[ERROR] El nombre de usuario ya esta en uso." << endl;
                } else {
                    sistemaUsuarios.insert(username);
                    cout << "\n[OK] Usuario '" << username << "' registrado exitosamente." << endl;
                }
                break;
            case 3:
                cout << "\nGracias por usar el programa. ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida. Intente de nuevo." << endl;
        }
    }
}

// =========================================================================
// LEVEL 2: MENÚ DE USUARIO (Gestión de sus Imágenes)
// =========================================================================
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
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int idImg;
                string nombreImg;
                cout << "\nIngrese un ID numerico para la nueva imagen: ";
                cin >> idImg;
                
                // Validar que el ID de la imagen sea único en la galería global
                if (galeriaGlobal.buscar(idImg) != nullptr) {
                    cout << "[ERROR] Ya existe una imagen con ese ID en el sistema." << endl;
                    break;
                }
                
                cout << "Ingrese el nombre de la imagen: ";
                cin.ignore(); // Limpiar el buffer de entrada antes de leer un string
                getline(cin, nombreImg);

                // 1. Insertamos en la galería global (Lista Circular Doble)
                galeriaGlobal.insertar(idImg, nombreImg);
                // 2. Registramos la autoría en la lista simple del usuario logueado
                usuarioActual->imagenesCreadas->insertar(idImg, nombreImg);
                
                cout << "[OK] Imagen creada correctamente." << endl;
                break;
            }
            case 2: {
                int idBuscar;
                cout << "\nIngrese el ID de la imagen que desea editar: ";
                cin >> idBuscar;

                // Buscamos la imagen en la galería circular doble global
                Imagen* imgEncontrada = galeriaGlobal.buscar(idBuscar);
                if (imgEncontrada != nullptr) {
                    cout << "\n[OK] Abriendo el editor para: " << imgEncontrada->nombre << endl;
                    menuEditor(imgEncontrada); // Pasar al nivel de edición de píxeles
                } else {
                    cout << "\n[ERROR] No se encontro ninguna imagen con el ID " << idBuscar << " en el sistema." << endl;
                }
                break;
            }
            case 3:
                cout << "\n--- HISTORIAL DE PROYECTOS DE " << usuarioActual->username << " ---" << endl;
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

// =========================================================================
// LEVEL 3: MENÚ DE EDICIÓN (Manipulación de Capas y Matrices Dispersas)
// =========================================================================
void menuEditor(Imagen* imagenActual) {
    int opcion = 0;

    while (opcion != 6) {
        cout << "\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "  EDITOR GRAPHIC: " << imagenActual->nombre << " (ID: " << imagenActual->id << ")" << endl;
        cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
        cout << "1. Agregar una capa a la imagen" << endl;
        cout << "2. Pintar un pixel en una capa" << endl;
        cout << "3. Ver listado de capas de esta imagen (AVL)" << endl;
        cout << "4. Ver secuencia de uso de capas (Lista Simple)" << endl;
        cout << "5. Visualizar matriz de pixeles de una capa" << endl;
        cout << "6. Salir del editor (Regresar)" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                int idCapa;
                string nombreCapa;
                cout << "\nIngrese el ID numerico para la capa: ";
                cin >> idCapa;
                
                if (imagenActual->arbolCapas->search(idCapa) != nullptr) {
                    cout << "[ERROR] Esta capa ya existe en el proyecto de la imagen." << endl;
                    break;
                }
                
                cout << "Ingrese el nombre de la capa (ej. Fondo, Silueta): ";
                cin >> nombreCapa;

                // 1. Añadimos la capa al árbol AVL propio de esta imagen
                imagenActual->arbolCapas->insert(idCapa, nombreCapa);
                // 2. La registramos en la secuencia de capas utilizadas (Lista Simple)
                imagenActual->capasUtilizadas->insertar(idCapa);
                
                cout << "[OK] Capa '" << nombreCapa << "' añadida con exito." << endl;
                break;
            }
            case 2: {
                int idCapa, fila, col;
                string colorHex;
                cout << "\n¿En que ID de capa desea pintar?: ";
                cin >> idCapa;

                // Buscamos el nodo de la capa en el árbol AVL de la imagen
                LayerNode* capa = imagenActual->arbolCapas->search(idCapa);
                if (capa != nullptr) {
                    cout << "Ingrese Fila: "; cin >> fila;
                    cout << "Ingrese Columna: "; cin >> col;
                    cout << "Ingrese Color en Hexadecimal (ej. #FF0000 o escriba '' para borrar): ";
                    cin >> colorHex;

                    // Accedemos a la matriz dispersa de esa capa específica e insertamos el píxel
                    capa->matriz->insert(fila, col, colorHex);
                    cout << "[OK] Pixel actualizado." << endl;
                } else {
                    cout << "[ERROR] La capa con ID " << idCapa << " no existe en esta imagen." << endl;
                }
                break;
            }
            case 3:
                cout << "\n--- Estructura AVL de Capas de la Imagen ---" << endl;
                imagenActual->arbolCapas->display();
                break;
            case 4:
                cout << "\nSecuencia logica de capas: ";
                imagenActual->capasUtilizadas->mostrarSecuencia();
                cout << endl;
                break;
            case 5: {
                int idCapa;
                cout << "\n¿De que capa desea ver el render de pixeles?: ";
                cin >> idCapa;

                LayerNode* capa = imagenActual->arbolCapas->search(idCapa);
                if (capa != nullptr) {
                    cout << "\n--- Visualizando Capa: " << capa->name << " (Matriz Dispersa 5x5) ---" << endl;
                    // Renderizamos un cuadrante de 5x5 por comodidad en consola
                    capa->matriz->display(5, 5); 
                } else {
                    cout << "[ERROR] Capa no encontrada." << endl;
                }
                break;
            }
            case 6:
                cout << "\nGuardando cambios y saliendo del editor..." << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion invalida." << endl;
        }
    }
}