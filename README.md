# Proyecto Final Integrador - Computación Paralela

**Grupo N°1**

**Integrantes:**
- Esains Julieta
- Espejo Adriel
- Haudet Felipe
- Izuel Tomás

**Universidad Tecnológica Nacional - LICPAD**

---

## Estructura del Proyecto

```
proyecto/
├── secuencial/
│   └── main.c          # Implementación secuencial
├── paralelo/
│   └── main.c          # Implementación paralela (MPI)
├── matrices.c          # Funciones de manejo de matrices
├── matrices.h          # Headers de matrices
├── primo.c             # Función es_primo()
├── Makefile            # Compilación y ejecución
└── README.md           # Este archivo
```

---

## Compilación

### Secuencial
```bash
gcc -Wall -Wextra -O2 -std=c99 -o programa_secuencial secuencial/main.c matrices.c
```

### Paralelo
```bash
mpicc -Wall -Wextra -O2 -std=c99 -o programa_paralelo paralelo/main.c matrices.c
```

O simplemente usar el Makefile:
```bash
make
```

---

## Ejecución

### Versión Secuencial
```bash
./programa_secuencial <M> <R>
```
**Ejemplo:**
```bash
./programa_secuencial 5000 5000
```

O con Make:
```bash
make run-secuencial M=5000 R=5000
```

### Versión Paralela
```bash
mpirun -n <num_procesos> ./programa_paralelo <M> <R>
```
**Ejemplo:**
```bash
mpirun -n 4 ./programa_paralelo 5000 5000
```

O con Make:
```bash
make run-paralelo NP=4 M=5000 R=5000
```

---

## Notas Importantes sobre la Implementación

### Memoria Contigua
Ambas versiones utilizan **memoria contigua** (`int*` en lugar de `int**`) para la matriz:
- Acceso a elementos: `matriz[i * R + j]`
- Permite envíos eficientes con MPI
- Reduce fragmentación de memoria

### Modelo Master-Worker
La versión paralela implementa:
- **Maestro (rank 0):** Crea la matriz completa, distribuye trabajo vía `MPI_Send`
- **Trabajadores (rank > 0):** Reciben su porción vía `MPI_Recv`, procesan y retornan
- **Recolección:** `MPI_Reduce` suma los conteos locales

### Distribución de Filas
- Cada proceso recibe **M/n filas** (división entera)
- El maestro procesa las **filas restantes** (M % n) para balancear
- División estática por bloques contiguos

### Medición de Tiempos
- **Secuencial:** Usa `clock()` de `time.h`
- **Paralelo:** Usa `MPI_Wtime()` para sincronización entre nodos

---

## Detalles Técnicos

### Función `es_primo()`
- Ubicada en `primo.c`
- Algoritmo básico: prueba divisibilidad desde 2 hasta n-1
- **Nota:** Puede optimizarse (ej: hasta √n) para matrices más grandes

### Generación de Números
- Rango: **1 a 1000** (configurable en `llenar_matriz()`)
- Usa `rand()` con seed basada en tiempo

### Parámetros de Prueba Recomendados
Según el informe:
- **Pequeña:** 5.000 x 5.000
- **Mediana:** 10.000 x 10.000
- **Grande:** 15.000 x 15.000

---

## Limitaciones Conocidas

1. **Memoria del Clúster:** Cada nodo tiene ~1.1 GB usable. No exceder matrices que requieran más memoria
2. **Sincronización:** `MPI_Barrier` antes y después del cómputo asegura mediciones precisas
3. **Seed Aleatoria:** Matrices diferentes en cada ejecución (para reproducibilidad, fijar seed)

---

## Resultados Esperados

Para validar la implementación, comparar con los tiempos del informe (Figura 3):

| Matriz | n=1 | n=2 | n=4 | n=8 |
|--------|-----|-----|-----|-----|
| 5k×5k  | 73.43s | 53.39s | 38.03s | 18.79s |
| 10k×10k | 292.74s | 192.28s | 80.43s | 48.11s |
| 15k×15k | 659.29s | 357.83s | 182.38s | 108.33s |

**Nota:** Los tiempos pueden variar según la carga del clúster y números aleatorios generados.

---

## Troubleshooting

### Error: "No se pudo asignar memoria"
- Reducir tamaño de matriz (M y R)
- Verificar memoria disponible: `free -h`

### Error de compilación con `mpicc`
- Verificar instalación de MPI: `which mpicc`
- Cargar módulo MPI si está en el clúster

### Tiempos muy diferentes al informe
- Normal debido a números aleatorios y carga del sistema
- El comportamiento relativo (speedup) debe ser similar

---

## Contacto

Para consultas sobre el código o resultados, contactar a cualquiera de los integrantes del Grupo N°1.
