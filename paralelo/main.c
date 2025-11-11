#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h> 
#include "../matrices.h"

// Función para procesar una porción de la matriz 
int procesar_porcion(int* porcion, int filas, int R) {
    int primos_encontrados = 0;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < R; j++) {
            if (es_primo(porcion[i * R + j])) {
                primos_encontrados++;
            }
        }
    }
    return primos_encontrados;
}


int main(int argc, char **argv) {
    int my_id, comm_size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_id);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size);

    if (argc != 3) {
        if (my_id == 0) {
            printf("Error: Se requieren 2 argumentos: M y R\n");
            printf("Ejemplo: mpirun -n 4 ./programa_paralelo 5000 5000\n");
        }
        MPI_Finalize();
        return 1;
    }

    const int M = atoi(argv[1]);
    const int R = atoi(argv[2]);
    int* matriz_completa = NULL;
    int conteo_local = 0;
    int conteo_total = 0;

    // ----- LÓGICA DEL MASTER-WORKER (rank 0) -----
    if (my_id == 0) {
        printf("Creando matriz %dx%d...\n", M, R);
        matriz_completa = crear_matriz(M, R); 
        if (matriz_completa == NULL) {
            printf("Error: No se pudo crear la matriz\n");
            MPI_Abort(MPI_COMM_WORLD, 1);
        }
        llenar_matriz(matriz_completa, M, R); 
        printf("Matriz creada.\n");
    }
    
    MPI_Barrier(MPI_COMM_WORLD);
    double start_time = MPI_Wtime();

    // 1. Calcular cuántas filas procesará cada uno
    int filas_por_proceso = M / comm_size;
    int filas_resto = M % comm_size;
    int mis_filas = (my_id == 0) ? (filas_por_proceso + filas_resto) : filas_por_proceso;
    int num_elementos_local = mis_filas * R;

    // 2. Crear buffer local 
    int* buffer_local = (int*)malloc(num_elementos_local * sizeof(int));
    if (buffer_local == NULL) {
        printf("Error: Proceso %d no pudo reservar buffer local\n", my_id);
        MPI_Abort(MPI_COMM_WORLD, 1);
    }

    // 3. Distribuir la matriz 
    if (my_id == 0) {
        // Maestro copia sus propias filas
        memcpy(buffer_local, matriz_completa, num_elementos_local * sizeof(int));
        
        // Maestro envía a cada TRABAJADOR su porción
        int fila_actual = mis_filas; 
        for (int i = 1; i < comm_size; i++) {
            int num_elementos_worker = filas_por_proceso * R;
            MPI_Send(&matriz_completa[fila_actual * R], 
                     num_elementos_worker, MPI_INT, i, 0, MPI_COMM_WORLD);
            fila_actual += filas_por_proceso;
        }
    } else {
        // TRABAJADORES reciben su porción
        MPI_Recv(buffer_local, num_elementos_local, MPI_INT, 
                 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    // 4. Procesar
    conteo_local = procesar_porcion(buffer_local, mis_filas, R);

    // 5. Recolectar resultados
    MPI_Reduce(&conteo_local, &conteo_total, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    MPI_Barrier(MPI_COMM_WORLD);
    double end_time = MPI_Wtime();
    double tiempo_resolucion = end_time - start_time;

    // 6. Imprimir y Limpiar
    if (my_id == 0) {
        printf("\n--- Resultados ---\n");
        printf("Matriz (%dx%d): %d primos - Tiempo: %f segundos\n", M, R, conteo_total, tiempo_resolucion);
        liberar_matriz(matriz_completa);
    }
    
    free(buffer_local); 
    
    MPI_Finalize();
    return 0;
}
