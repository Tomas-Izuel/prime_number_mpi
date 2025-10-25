#include <stdio.h>
#include <time.h>
#include "../matrices.h"

// Función para procesar una matriz secuencialmente y contar primos
int procesar_matriz_secuencial(int **matriz, int n) {
    if (matriz == NULL) return 0;
    
    int primos_encontrados = 0;
    
    // Procesar toda la matriz secuencialmente
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            int valor = matriz[i][j];
            if (es_primo(valor)) {
                primos_encontrados++;
            }
        }
    }
    
    return primos_encontrados;
}

int main() {
    
    printf("Creando matrices...\n");
    int **matriz_pequena = NULL, **matriz_mediana = NULL, **matriz_grande = NULL;
    crear_matrices_especificas(&matriz_pequena, &matriz_mediana, &matriz_grande);
    
    clock_t start_time = clock();
    
    int total_primos_pequena = procesar_matriz_secuencial(matriz_pequena, 1000);
    
    int total_primos_mediana = procesar_matriz_secuencial(matriz_mediana, 5000);
    
    int total_primos_grande = procesar_matriz_secuencial(matriz_grande, 10000);
    
    clock_t end_time = clock();
    double tiempo_resolucion = ((double)(end_time - start_time)) / CLOCKS_PER_SEC;
    
    printf("Matriz pequeña (1000x1000): %d primos\n", total_primos_pequena);
    printf("Matriz mediana (5000x5000): %d primos\n", total_primos_mediana);
    printf("Matriz grande (10000x10000): %d primos\n", total_primos_grande);
    printf("Tiempo de resolución: %f segundos\n", tiempo_resolucion);
    
    // Liberar memoria de las matrices
    liberar_todas_las_matrices(matriz_pequena, matriz_mediana, matriz_grande);
    
    return 0;
}
