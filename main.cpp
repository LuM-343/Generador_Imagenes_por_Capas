#include <iostream>
#include <string>
#include "ArbolUsuarios.h"
#include "ListaImagenes.h"
#include "ArbolCapas.h"
#include "MatrizDispersa.h"
#include "CargaMasiva.h"

using namespace std;

// Declaración de estructuras globales necesarias para todo el sistema
UserAVLTree sistemaUsuarios;
ListaCircularDobleImagenes galeriaGlobal;
LayerAVLTree arbolCapasGlobal;

// Prototipos de funciones de los menús
void menuInicio();
void menuUsuario(UserNode* usuarioActual);
void menuEditor(Imagen* imagenActual);
void menuReportes();
void menuCargaMasiva();

int main() {
    // Iniciar el sistema
    menuInicio();
    return 0;
}

// =========================================================================
// LEVEL 1: MENÚ DE INICIO (Login / Registro / Reportes / Carga Masiva)
// =========================================================================
void menuInicio() {
    int opcion = 0;
    string username;

    while (opcion != 5) { // Cambiamos a 5 opciones
        cout << "\n=========================================" << endl;
        cout << "         EDITOR DE PIXEL ART - INICIO    " << endl;
        cout << "=========================================" << endl;
        cout << "1. Iniciar Sesion" << endl;
        cout << "2. Registrar nuevo usuario" << endl;
        cout << "3. Reportes de Estructuras (Graphviz)" << endl; 
        cout << "4. Carga Masiva de Datos" << endl; // <-- NUEVA OPCIÓN
        cout << "5. Salir del programa" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1: {
                cout << "\nIngrese su nombre de usuario: ";
                cin >> username;
                
                UserNode* usuarioLogueado = sistemaUsuarios.search(username);
                if (usuarioLogueado != nullptr) {
                    cout << "\n[OK] Bienvenido de nuevo, " << username << "!" << endl;
                    menuUsuario(usuarioLogueado);
                } else {
                    cout << "\n[ERROR] El usuario '" << username << "' no existe." << endl;
                }
                break;
            }
            case 2:
                cout << "\nIngrese el nombre para el nuevo usuario: ";
                cin >> username;
                if (sistemaUsuarios.search(username) != nullptr) {
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
                cout << "\nGracias por usar el programa. ¡Hasta pronto!" << endl;
                break;
            default:
                cout << "\n[ERROR] Opcion no valida. Intente de nuevo." << endl;
        }
    }
}

// =========================================================================
// SUBMENÚ: CARGA MASIVA DE DATOS
// =========================================================================
void menuCargaMasiva() {
    int opcion = 0;
    string rutaArchivo;

    while (opcion != 4) {
        cout << "\n--- CARGA MASIVA DE DATOS ---" << endl;
        cout << "Recuerde el orden de carga: 1. Capas -> 2. Imagenes -> 3. Usuarios" << endl;
        cout << "1. Cargar archivo de Capas (.cap)" << endl;
        cout << "2. Cargar archivo de Imagenes (.im)" << endl;
        cout << "3. Cargar archivo de Usuarios (.usr)" << endl;
        cout << "4. Regresar al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        
        // Seguro contra letras o errores en el buffer
        if (!(cin >> opcion)) {
            cin.clear(); 
            cin.ignore(10000, '\n'); 
            cout << "\n[ERROR] Entrada no valida. Por favor, ingrese un numero." << endl;
            continue; // Reinicia el ciclo
        }

        switch (opcion) {
            case 1:
                cout << "\nIngrese la ruta del archivo de capas (.cap): ";
                cin.ignore(); // Limpiamos el salto de linea ('\n') antes de usar getline
                getline(cin, rutaArchivo); // Se lee toda la ruta evitando espacios
                
                //Previene el error de copiar la ruta de acceso con las comillas
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"' && rutaArchivo.back() == '"') {
                    rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                }
                
                CargaMasiva::cargarCapas(rutaArchivo, arbolCapasGlobal);
                break;
            
            case 2:
                cout << "\nIngrese la ruta del archivo de imagenes (.im): ";
                cin.ignore(); 
                getline(cin, rutaArchivo);
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"' && rutaArchivo.back() == '"') {
                    rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                }
                CargaMasiva::cargarImagenes(rutaArchivo, galeriaGlobal);
                break;

            case 3:
                cout << "\nIngrese la ruta del archivo de usuarios (.usr): ";
                cin.ignore();
                getline(cin, rutaArchivo);
                if (!rutaArchivo.empty() && rutaArchivo.front() == '"' && rutaArchivo.back() == '"') {
                    rutaArchivo = rutaArchivo.substr(1, rutaArchivo.length() - 2);
                }
                CargaMasiva::cargarUsuarios(rutaArchivo, sistemaUsuarios, galeriaGlobal);
                break;

            case 4:
                cout << "\nRegresando al menu principal..." << endl;
                break;

            default:
                cout << "\n[ERROR] Opcion no valida." << endl;
        }
    }
}

// =========================================================================
// SUBMENÚ: REPORTES DEL ESTADO DE MEMORIA
// =========================================================================
void menuReportes() {
    int opcion = 0;

    while (opcion != 6) {
        cout << "\n--- REPORTES DE MEMORIA (GRAPHVIZ) ---" << endl;
        cout << "1. Ver Arbol AVL de Usuarios" << endl;
        cout << "2. Ver Lista Circular Doble de Imagenes" << endl;
        cout << "3. Ver Arbol AVL de Capas de una Imagen" << endl;
        cout << "4. Ver Matriz Dispersa de una Capa" << endl;
        cout << "5. Ver Lista Simple de Imagenes de un Usuario" << endl;
        cout << "6. Regresar al Menu Principal" << endl;
        cout << "Seleccione una opcion: ";
        cin >> opcion;

        switch (opcion) {
            case 1:
                cout << "\nGenerando reporte de Usuarios..." << endl;
                sistemaUsuarios.graficarArbol();
                break;
            
            case 2:
                cout << "\nGenerando reporte de la Galeria Global..." << endl;
                galeriaGlobal.graficarLista();
                break;

            case 3: {
                int idImg;
                cout << "\nIngrese el ID de la imagen para ver su arbol de capas: ";
                cin >> idImg;
                Imagen* img = galeriaGlobal.buscar(idImg);
                if (img != nullptr) {
                    img->arbolCapas->graficarArbol("ArbolCapas_Img_" + to_string(idImg));
                } else {
                    cout << "[ERROR] Imagen no encontrada." << endl;
                }
                break;
            }

            case 4: {
                int idImg, idCapa;
                cout << "\nIngrese el ID de la imagen dueña de la capa: ";
                cin >> idImg;
                Imagen* img = galeriaGlobal.buscar(idImg);
                
                if (img != nullptr) {
                    cout << "Ingrese el ID de la capa a graficar: ";
                    cin >> idCapa;
                    LayerNode* capa = img->arbolCapas->search(idCapa);
                    if (capa != nullptr) {
                        capa->matriz->graficarMatriz("Matriz_Img" + to_string(idImg) + "_Capa" + to_string(idCapa));
                    } else {
                        cout << "[ERROR] Capa no encontrada en esta imagen." << endl;
                    }
                } else {
                    cout << "[ERROR] Imagen no encontrada." << endl;
                }
                break;
            }

            case 5: {
                string user;
                cout << "\nIngrese el nombre del usuario: ";
                cin >> user;
                UserNode* nodoUser = sistemaUsuarios.search(user);
                if (nodoUser != nullptr) {
                    nodoUser->imagenesCreadas->graficarLista(user);
                } else {
                    cout << "[ERROR] Usuario no encontrado." << endl;
                }
                break;
            }

            case 6:
                cout << "\nRegresando al menu principal..." << endl;
                break;

            default:
                cout << "\n[ERROR] Opcion no valida." << endl;
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