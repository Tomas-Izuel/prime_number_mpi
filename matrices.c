#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include "matrices.h"
#include "primo.c" 

// Función para crear una matriz contigua de M x R
int* crear_matriz(int M, int R) {
    int* matriz = (int*)malloc(M * R * sizeof(int));
    if (matriz == NULL) {
        printf("Error: No se pudo asignar memoria para la matriz\n");
        return NULL;
    }
    return matriz;
}

// Función para llenar una matriz contigua con valores aleatorios
void llenar_matriz(int* matriz, int M, int R) {
    srand(time(NULL));
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < R; j++) {
            // Usamos la fórmula de acceso contiguo
            matriz[i * R + j] = (rand() % 1000) + 1; 
        }
    }
}

// Función para liberar memoria de una matriz contigua
void liberar_matriz(int* matriz) {
    free(matriz);
}
