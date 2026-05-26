# Proyecto: Generador de Imágenes por Capas

## Descripción
[cite_start]Este repositorio contiene el código fuente del **Generador de Imágenes por Capas**, una aplicación de consola desarrollada nativamente en C++ para el curso de Estructura de Datos I de la Universidad Rafael Landívar[cite: 89]. [cite_start]Su objetivo principal es generar arte basado en píxeles (Pixel Art) mediante la superposición matemática de distintas capas almacenadas en la memoria[cite: 90].

## Estructuras de Datos Implementadas
[cite_start]El núcleo del sistema fue desarrollado desde cero gestionando punteros y memoria dinámica, omitiendo por completo el uso de librerías de colecciones estándar (STL)[cite: 91].

* [cite_start]**Matrices Dispersas Ortogonales:** Optimizan el almacenamiento de los lienzos al guardar únicamente los colores hexadecimales en las coordenadas exactas donde existe un píxel[cite: 91].
* [cite_start]**Árboles Binarios (ABB y AVL):** Un Árbol Binario de Búsqueda organiza el catálogo global de capas, mientras que un árbol AVL gestiona los registros de usuarios para garantizar tiempos de búsqueda logarítmicos[cite: 92].
* [cite_start]**Listas Enlazadas:** Una Lista Circular Doblemente Enlazada administra el catálogo de imágenes[cite: 93]. [cite_start]Listas simples enlazan las dependencias de capas utilizando apuntadores en lugar de duplicar datos[cite: 94].

## Características Principales
* [cite_start]**Carga Masiva de Datos:** Inicialización rápida del sistema mediante la lectura de archivos de configuración con extensiones `.cap`, `.im` y `.usr`[cite: 95].
* [cite_start]**Renderizado Web:** Exportación y superposición de matrices a formato HTML bajo cuatro modalidades: recorrido de árboles (Preorden, Inorden, Postorden), por lista de imágenes, por capa individual o por galería de usuario[cite: 96].
* [cite_start]**Auditoría de Memoria:** Generación de reportes gráficos del estado de las estructuras internas en tiempo de ejecución, apoyándose en la herramienta Graphviz[cite: 97].
* [cite_start]**Operaciones CRUD:** Módulo de mantenimiento completo para la gestión de agregar, modificar y eliminar perfiles de usuario e imágenes en el catálogo[cite: 98].

## Requisitos y Compilación
* [cite_start]**Lenguaje:** C++[cite: 99].
* [cite_start]**Herramientas Externas:** Es indispensable tener instalado Graphviz en el sistema para que las funciones de reportes operen correctamente[cite: 99].
* [cite_start]**Compilación:** Para compilar la aplicación, asegúrate de procesar el archivo `main.cpp` junto a todos sus correspondientes archivos `.h` en tu compilador preferido[cite: 100]. [cite_start]Si organizaste los encabezados en una subcarpeta (ej. `estructuras`), recuerda agregar la bandera de ruta de inclusión (ej. `-I./estructuras`) al momento de compilar por consola[cite: 101].
