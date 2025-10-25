#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "../primo.c"
#include "../matrices.h"

// En los parametros asumo que paso
// argv[0] => Nombre del programa
// argv[1] => Tipo de matriz (pequena, mediana, grande)
int main(int argc, char **argv){
    /***** Inicializo MPI *****/
    int my_id, comm_size;
    MPI_Init(&argc, &argv);

    // Obtengo el id del proceso
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    // Obtengo el total de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    // CREACIÓN DE MATRICES (NO INCLUIDA EN TIEMPO DE RESOLUCIÓN)
    int **matriz_pequena = NULL, **matriz_mediana = NULL, **matriz_grande = NULL;
    
    if (my_id == 0) {
        crear_matrices_especificas(&matriz_pequena, &matriz_mediana, &matriz_grande);
    }
    
    // Sincronización antes de comenzar el procesamiento
    MPI_Barrier(MPI_COMM_WORLD);
    
    // INICIO DEL TIEMPO DE RESOLUCIÓN
    double start_time = MPI_Wtime();

    // Procesar las 3 matrices
    int total_primos_pequena = 0, total_primos_mediana = 0, total_primos_grande = 0;
    
    total_primos_pequena = procesar_matriz(matriz_pequena, 1000, my_id, comm_size);
    
    total_primos_mediana = procesar_matriz(matriz_mediana, 5000, my_id, comm_size);
    
    total_primos_grande = procesar_matriz(matriz_grande, 10000, my_id, comm_size);

    double end_time = MPI_Wtime();
    
    if (my_id == 0) {
        printf("Matriz pequeña (1000x1000): %d primos\n", total_primos_pequena);
        printf("Matriz mediana (5000x5000): %d primos\n", total_primos_mediana);
        printf("Matriz grande (10000x10000): %d primos\n", total_primos_grande);
        printf("Tiempo de resolución: %f segundos\n", end_time - start_time);
    }

    // Liberar memoria de las matrices
    if (my_id == 0) {
        liberar_todas_las_matrices(matriz_pequena, matriz_mediana, matriz_grande);
    }
    
    MPI_Finalize();
    return 0;
}