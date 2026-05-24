#include <iostream>
#include "ListaImagenes.h"
#include "ArbolUsuarios.h"

using namespace std;

int main() {
    // 1. Instanciamos el árbol de usuarios global y la galería de imágenes global
    UserAVLTree sistemaUsuarios;
    ListaCircularDobleImagenes galeriaGlobal;

    cout << "=== INTEGRACION DE LISTAS SIMPLES ===" << endl;

    // 2. Simulamos la creación de un usuario
    sistemaUsuarios.insert("carlos_art");
    
    // 3. Buscamos al usuario en el sistema (Para efectos de prueba, simularemos la asignación directa)
    // En tu lógica real buscarás el nodo en el AVL, aquí accederemos conceptualmente:
    cout << "\nAsignando imagenes creadas al usuario 'carlos_art'..." << endl;
    
    // Creamos una lista de imágenes simulada para el usuario
    ListaSimpleImagenes* listaDeCarlos = new ListaSimpleImagenes();
    listaDeCarlos->insertar(101, "Pixel_Art_Sword");
    listaDeCarlos->insertar(102, "Castle_Background");

    cout << "\nImagenes hechas por el usuario:" << endl;
    listaDeCarlos->mostrar();


    // 4. Simulamos una Imagen con su lista de capas utilizadas
    cout << "\nConfigurando capas utilizadas para la imagen 'Pixel_Art_Sword'..." << endl;
    Imagen* nuevaImagen = new Imagen(101, "Pixel_Art_Sword");
    
    // Añadimos qué capas usa esta imagen en su lista simple
    nuevaImagen->capasUtilizadas->insertar(1); // Capa de fondo
    nuevaImagen->capasUtilizadas->insertar(2); // Capa de silueta
    nuevaImagen->capasUtilizadas->insertar(5); // Capa de brillos/detalles

    cout << "Secuencia de capas utilizadas por la imagen: ";
    nuevaImagen->capasUtilizadas->mostrarSecuencia();
    cout << endl;

    // Buscar un usuario para "iniciar sesión"
    UserNode* usuarioActual = sistemaUsuarios.search("carlos_art");

    if (usuarioActual != nullptr) {
        cout << "Bienvenido de nuevo, " << usuarioActual->username << "!" << endl;
        // Ahora sí podemos insertar directamente en SU lista de imagenes
        usuarioActual->imagenesCreadas->insertar(101, "Pixel_Art_Sword");
    } else {
        cout << "Error: Usuario no encontrado." << endl;
    }

    // Buscar una imagen para editarla
    Imagen* imagenAEditar = galeriaGlobal.buscar(101);

    if (imagenAEditar != nullptr) {
        cout << "Abriendo imagen: " << imagenAEditar->nombre << "..." << endl;
        // Aquí podrías acceder a su árbol de capas
        imagenAEditar->arbolCapas->insert(1, "Fondo");
    }

    return 0;
}