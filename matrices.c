#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <mpi.h>
#include "primo.c"

// Función para crear una matriz cuadrada de tamaño n x n
int** crear_matriz(int n) {
    int **matriz = (int**)malloc(n * sizeof(int*));
    if (matriz == NULL) {
        printf("Error: No se pudo asignar memoria para la matriz\n");
        return NULL;
    }
    
    for (int i = 0; i < n; i++) {
        matriz[i] = (int*)malloc(n * sizeof(int));
        if (matriz[i] == NULL) {
            printf("Error: No se pudo asignar memoria para la fila %d\n", i);
            // Liberar memoria ya asignada
            for (int j = 0; j < i; j++) {
                free(matriz[j]);
            }
            free(matriz);
            return NULL;
        }
    }
    
    return matriz;
}

// Función para llenar una matriz con valores aleatorios
void llenar_matriz(int **matriz, int n) {
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            // Generar números aleatorios entre 1 y 1000
            matriz[i][j] = (rand() % 1000) + 1;
        }
    }
}

// Función para liberar memoria de una matriz
void liberar_matriz(int **matriz, int n) {
    if (matriz != NULL) {
        for (int i = 0; i < n; i++) {
            free(matriz[i]);
        }
        free(matriz);
    }
}

// Función para imprimir una matriz (solo para matrices pequeñas)
void imprimir_matriz(int **matriz, int n) {
    if (n > 10) {
        printf("Matriz muy grande para imprimir (n=%d)\n", n);
        return;
    }
    
    printf("Matriz %dx%d:\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%4d ", matriz[i][j]);
        }
        printf("\n");
    }
}

// Función para crear las tres matrices según especificación
void crear_matrices_especificas(int ***matriz_pequena, int ***matriz_mediana, int ***matriz_grande) {
    // Matriz pequeña: 1.000 x 1.000
    *matriz_pequena = crear_matriz(1000);
    if (*matriz_pequena != NULL) {
        llenar_matriz(*matriz_pequena, 1000);
    }
    
    // Matriz mediana: 5.000 x 5.000
    *matriz_mediana = crear_matriz(5000);
    if (*matriz_mediana != NULL) {
        llenar_matriz(*matriz_mediana, 5000);
    }
    
    // Matriz grande: 10.000 x 10.000
    *matriz_grande = crear_matriz(10000);
    if (*matriz_grande != NULL) {
        llenar_matriz(*matriz_grande, 10000);
    }
}

// Función para procesar una matriz y contar primos
int procesar_matriz(int **matriz, int n, int my_id, int comm_size) {
    if (matriz == NULL) return 0;
    
    // Crear matriz en todos los procesos
    int **matriz_local = NULL;
    if (my_id != 0) {
        matriz_local = crear_matriz(n);
    } else {
        matriz_local = matriz;
    }
    
    // Enviar la matriz a todos los procesos usando MPI_Bcast
    for (int i = 0; i < n; i++) {
        MPI_Bcast(matriz_local[i], n, MPI_INT, 0, MPI_COMM_WORLD);
    }
    
    // Sincronización después del broadcast
    MPI_Barrier(MPI_COMM_WORLD);

    // Cada proceso procesa su parte de la matriz
    int elementos_por_proceso = (n * n) / comm_size;
    int inicio = my_id * elementos_por_proceso;
    int fin = (my_id == comm_size - 1) ? (n * n) : inicio + elementos_por_proceso;

    // Verificar números primos en la parte asignada de la matriz
    int primos_encontrados = 0;
    for (int i = inicio; i < fin; i++) {
        int fila = i / n;
        int columna = i % n;
        int valor = matriz_local[fila][columna];
        
        if (es_primo(valor)) {
            primos_encontrados++;
        }
    }

    // Sincronización antes de la recolección de resultados
    MPI_Barrier(MPI_COMM_WORLD);

    // Recolectar el total de primos encontrados
    int total_primos;
    MPI_Reduce(&primos_encontrados, &total_primos, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    // Liberar matriz local si no es la original
    if (my_id != 0 && matriz_local != NULL) {
        liberar_matriz(matriz_local, n);
    }
    
    return total_primos;
}

// Función para liberar todas las matrices
void liberar_todas_las_matrices(int **matriz_pequena, int **matriz_mediana, int **matriz_grande) {
    if (matriz_pequena != NULL) {
        liberar_matriz(matriz_pequena, 1000);
    }
    
    if (matriz_mediana != NULL) {
        liberar_matriz(matriz_mediana, 5000);
    }
    
    if (matriz_grande != NULL) {
        liberar_matriz(matriz_grande, 10000);
    }
}
