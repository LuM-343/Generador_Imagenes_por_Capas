# Generador_Imagenes_por_Capas
##Descripción
Este repositorio contiene el código fuente del Generador de Imágenes por Capas, una aplicación de consola desarrollada nativamente en C++ para el curso de Estructura de Datos I de la Universidad Rafael Landívar. Su objetivo principal es generar arte basado en píxeles (Pixel Art) mediante la superposición matemática de distintas capas almacenadas en la memoria.

##Estructuras de Datos Implementadas
El núcleo del sistema fue desarrollado desde cero gestionando punteros y memoria dinámica, omitiendo por completo el uso de librerías de colecciones estándar (STL):
*Matrices Dispersas Ortogonales: Optimizan el almacenamiento de los lienzos al guardar únicamente los colores hexadecimales en las coordenadas exactas donde existe un píxel.
*Árboles Binarios (ABB y AVL): Un Árbol Binario de Búsqueda organiza el catálogo global de capas, mientras que un árbol AVL gestiona los registros de usuarios para garantizar tiempos de búsqueda logarítmicos.
*Listas Enlazadas: Una Lista Circular Doblemente Enlazada administra el catálogo de imágenes. 
*Listas simples enlazan las dependencias de capas utilizando apuntadores en lugar de duplicar datos.

##Características Principales
*Carga Masiva de Datos: Inicialización rápida del sistema mediante la lectura de archivos de configuración con extensiones .cap, .im y .usr.  
*Renderizado Web: Exportación y superposición de matrices a formato HTML bajo cuatro modalidades: recorrido de árboles (Preorden, Inorden, Postorden), por lista de imágenes, por capa individual o por galería de usuario.  
*Auditoría de Memoria: Generación de reportes gráficos del estado de las estructuras internas en tiempo de ejecución, apoyándose en la herramienta Graphviz.
*Operaciones CRUD: Módulo de mantenimiento completo para la gestión de agregar, modificar y eliminar perfiles de usuario e imágenes en el catálogo.  

##Requisitos y Compilación
*Lenguaje: C++.  
*Herramientas Externas: Es indispensable tener instalado Graphviz en el sistema para que las funciones de reportes operen correctamente.  
*Para compilar la aplicación, asegúrate de tener todos los archivos del repositorio incluida la carpeta estructuras.
