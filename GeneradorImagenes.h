#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "estructuras/MatrizDispersa.h"
#include "estructuras/ArbolCapas.h"

using namespace std;

class GeneradorImagenes {
public:
    
    static void exportarHTML(string nombreArchivo, vector<LayerNode*> capasAUtilizar) {
        ofstream archivo(nombreArchivo + ".html");

        // Si no hay capas, se genera un pixel negro.
        if (capasAUtilizar.empty()) {
            archivo << "<!DOCTYPE html>\n<html>\n<head>\n<style>\n";
            archivo << "  .lienzo { display: grid; grid-template-columns: repeat(1, 20px); gap: 1px; background-color: #ccc; width: max-content; border: 2px solid black; }\n";
            archivo << "  .pixel { width: 20px; height: 20px; background-color: #000000; }\n";
            archivo << "</style>\n</head>\n<body>\n";
            archivo << "<h2>Imagen Exportada: " << nombreArchivo << " (Sin Capas)</h2>\n";
            archivo << "<div class='lienzo'>\n  <div class='pixel'></div>\n</div>\n</body>\n</html>";
            archivo.close();
            cout << "[OK] Imagen '" << nombreArchivo << "' generada (Pixel Negro por falta de capas)." << endl;
            return;
        }

        // 1. Fusionar todas las capas en una sola Matriz Dispersa y calcular dimensiones
        LayerMatrix lienzoFinal;
        int maxFilas = 0, maxCols = 0;

        for (LayerNode* capa : capasAUtilizar) {
            // Obtenemos el tamaño máximo que alcanzó el dibujo
            capa->matriz->obtenerDimensionesMaximas(maxFilas, maxCols);
            // Superponemos los pixeles de esta capa sobre el lienzo final
            capa->matriz->superponerEn(&lienzoFinal);
        }

        // 2. Generar el archivo HTML
        archivo << "<!DOCTYPE html>\n<html>\n<head>\n<style>\n";
        // Calculamos las columnas para el Grid
        archivo << "  .lienzo { display: grid; grid-template-columns: repeat(" << maxCols << ", 20px); gap: 1px; background-color: #ccc; width: max-content; border: 2px solid black; }\n";
        // Fondo blanco para las áreas sin color
        archivo << "  .pixel { width: 20px; height: 20px; background-color: #ffffff; }\n";
        archivo << "</style>\n</head>\n<body>\n";
        archivo << "<h2>Imagen Exportada: " << nombreArchivo << "</h2>\n";
        archivo << "<div class='lienzo'>\n";

        // 3. Pintar cada cuadrante (desde 1 hasta el máximo)
        for (int i = 1; i <= maxFilas; i++) {
            for (int j = 1; j <= maxCols; j++) {
                Node* pixel = lienzoFinal.search(i, j);
                if (pixel != nullptr) {
                    archivo << "  <div class='pixel' style='background-color: " << pixel->color << ";'></div>\n";
                } else {
                    archivo << "  <div class='pixel'></div>\n"; 
                }
            }
        }

        archivo << "</div>\n</body>\n</html>";
        archivo.close();

        cout << "[OK] Arte generado exitosamente en: '" << nombreArchivo << ".html'." << endl;
    }
};