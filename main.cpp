#include <iostream>
#include "ListaImagenes.h"
#include "ArbolUsuarios.h"

using namespace std;

int main() {
    // Instanciamos las estructuras generales
    ListaCircularDobleImagenes galeria;
    UserAVLTree usuarios;

    cout << "=== PRUEBA DE ESTRUCTURAS INTEGRADAS ===" << endl;

    // 1. Probamos Usuarios
    usuarios.insert("carlos_admin");
    usuarios.insert("ana_user");
    usuarios.insert("beto_dev");
    
    cout << "\nUsuarios registrados:" << endl;
    usuarios.display();

    // 2. Probamos Imagenes
    galeria.insert(1, "MarioBros_PixelArt");
    galeria.insert(2, "Paisaje_Retro");
    
    galeria.mostrar();

    return 0;
}