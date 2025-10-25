#ifndef MATRICES_H
#define MATRICES_H

// Función para crear una matriz cuadrada de tamaño n x n
int** crear_matriz(int n);

// Función para llenar una matriz con valores aleatorios
void llenar_matriz(int **matriz, int n);

// Función para liberar memoria de una matriz
void liberar_matriz(int **matriz, int n);

// Función para imprimir una matriz (solo para matrices pequeñas)
void imprimir_matriz(int **matriz, int n);

// Función para crear las tres matrices según especificación
void crear_matrices_especificas(int ***matriz_pequena, int ***matriz_mediana, int ***matriz_grande);

// Función para procesar una matriz y contar primos
int procesar_matriz(int **matriz, int n, int my_id, int comm_size);

// Función para liberar todas las matrices
void liberar_todas_las_matrices(int **matriz_pequena, int **matriz_mediana, int **matriz_grande);

#endif
