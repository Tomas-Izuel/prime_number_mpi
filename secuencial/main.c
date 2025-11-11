#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include "../matrices.h"

// Función para procesar una matriz secuencialmente 
int procesar_matriz_secuencial(int* matriz, int M, int R) {
    if (matriz == NULL) return 0;
    
    int primos_encontrados = 0;
    
    // Procesar toda la matriz secuencialmente
    for (int i = 0; i < M; i++) {
        for (int j = 0; j < R; j++) {
            int valor = matriz[i * R + j]; 
            if (es_primo(valor)) {
                primos_encontrados++;
            }
        }
    }
    return primos_encontrados;
}

int main(int argc, char **argv) {
    
    if (argc != 3) {
        printf("Error: Se requieren 2 argumentos: M y R\n");
        printf("Ejemplo: ./programa_secuencial 5000 5000\n");
        return 1;
    }

    const int M = atoi(argv[1]);
    const int R = atoi(argv[2]);
    
    printf("Creando matriz %dx%d...\n", M, R);
    int* matriz = crear_matriz(M, R);
    if (matriz == NULL) return 1;
    
    llenar_matriz(matriz, M, R);
    printf("Matriz creada\n");

    clock_t start_time = clock();
    
    int total_primos = procesar_matriz_secuencial(matriz, M, R);
    
    clock_t end_time = clock();
    double tiempo_resolucion = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("\n--- Resultados ---\n");
    printf("Matriz (%dx%d): %d primos - Tiempo: %f segundos\n", M, R, total_primos, tiempo_resolucion);
    
    liberar_matriz(matriz);
    
    return 0;
}
