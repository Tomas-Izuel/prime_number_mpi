#ifndef MATRICES_H
#define MATRICES_H

#include <stdbool.h> 

// Función auxiliar de primo.c
bool es_primo(int num);

// Función para crear una matriz contigua de M x R
int* crear_matriz(int M, int R);

// Función para llenar una matriz con valores aleatorios
void llenar_matriz(int* matriz, int M, int R);

// Función para liberar memoria de una matriz contigua
void liberar_matriz(int* matriz);

#endif
