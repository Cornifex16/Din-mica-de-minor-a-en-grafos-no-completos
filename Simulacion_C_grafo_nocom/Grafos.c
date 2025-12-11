#include <stdio.h>
#include <stdlib.h>
#include "Grafos.h"
#include "rng.h"

Nodo* Crear_grafo(int n, int k, uint64_t *rng_state) {
    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
        int num_vec = k + rand_uniform(rng_state, n - k);
        nodos[i].num_vecinos = num_vec;
        nodos[i].vecinos = malloc(num_vec * sizeof(int));

        int usados = 0;
        for (int j = 0; j < num_vec; j++) {
            int indice;
            int repetido;
            do {
                indice = rand_uniform(rng_state, n);
                repetido = (indice == i);
                for (int t = 0; t < usados && !repetido; t++){
                    if (nodos[i].vecinos[t] == indice) {
                        repetido = 1;
                    }
                }
            } while (repetido);
            nodos[i].vecinos[usados++] = indice;
        }
    }

    simetrizar_grafo(nodos, n);

    return nodos;
}

Nodo* Crear_grafo_alt(int n, int k, uint64_t *rng_state) {
    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) return NULL;

    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
        nodos[i].num_vecinos = 0;
        nodos[i].vecinos = NULL;
    }

    int *grados_objetivo = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        grados_objetivo[i] = k + rand_uniform(rng_state, n - k);
    }

    for (int i = 0; i < n; i++) {
        while (nodos[i].num_vecinos < grados_objetivo[i]) {
            int j = rand_uniform(rng_state, n);
            if (j == i) continue;

            int repetido = 0;
            for (int u = 0; u < nodos[i].num_vecinos && !repetido; u++)
                if (nodos[i].vecinos[u] == j) repetido = 1;
            if (repetido) continue;

            nodos[i].vecinos = realloc(nodos[i].vecinos,
                                       (nodos[i].num_vecinos + 1) * sizeof(int));
            nodos[i].vecinos[nodos[i].num_vecinos++] = j;

            nodos[j].vecinos = realloc(nodos[j].vecinos,
                                       (nodos[j].num_vecinos + 1) * sizeof(int));
            nodos[j].vecinos[nodos[j].num_vecinos++] = i;
        }
    }

    free(grados_objetivo);
    return nodos;
}


void simetrizar_grafo(Nodo *nodos, int n) {
    for (int i = 0; i < n; i++) {
        for (int t = 0; t < nodos[i].num_vecinos; t++) {
            int j = nodos[i].vecinos[t];

            int existe = 0;
            for (int x = 0; x < nodos[j].num_vecinos; x++) {
                if (nodos[j].vecinos[x] == i) {
                    existe = 1;
                    break;
                }
            }

            if (!existe) {
                nodos[j].vecinos = realloc(nodos[j].vecinos, (nodos[j].num_vecinos + 1) * sizeof(int));
                nodos[j].vecinos[nodos[j].num_vecinos++] = i;
            }
        }
    }
}

Nodo* crear_grafo_erdosrenyi(int n, int p, uint64_t *rng_state) {
    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
        nodos[i].num_vecinos = 0;
        nodos[i].vecinos = NULL;
    }

    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++){
            // probabilidad medida en enteros entre 0 y 100 en un inicio
            // afinar la probabilidad en un rango mas pequeño añadiendo dos 0
            int probabilidad_union = (rand_uniform(rng_state, 10000) < p);
            if (probabilidad_union) {
                if (j == i) continue;
                nodos[i].num_vecinos++;
                nodos[i].vecinos = realloc(nodos[i].vecinos, nodos[i].num_vecinos * sizeof(int));
                if (!nodos[i].vecinos) {
                    printf("error en realloc vecinos de i, dupla: (%d,%d)\n", i,j);
                    return NULL;
                }
                nodos[i].vecinos[nodos[i].num_vecinos - 1] = j;
                nodos[j].num_vecinos++;
                nodos[j].vecinos = realloc(nodos[j].vecinos, nodos[j].num_vecinos * sizeof(int));
                if (!nodos[i].vecinos) {
                    printf("error en realloc vecinos de j\n");
                    return NULL;
                }
                nodos[j].vecinos[nodos[j].num_vecinos - 1] = i;
            }
        }
    }
    printf("se revisa la conectividad: %d\n", Revisar_conectividad(nodos, n));
    printf("entrega el grafo");
    if (!Revisar_conectividad(nodos, n)) {
        printf("no se creo un grado conexo, reintentar");
        nodos = crear_grafo_erdosrenyi(n, p, rng_state);
        printf("se revisa la conectividad: %d\n", Revisar_conectividad(nodos, n));
    }
    return nodos;
}

int agregar_arista(Nodo *nodo, int vecino) {
    nodo->num_vecinos++;
    int *temp = realloc(nodo->vecinos, nodo->num_vecinos * sizeof(int));
    if (!temp) {
        nodo->num_vecinos--;
        return -1;
    }
    nodo->vecinos = temp;
    nodo->vecinos[nodo->num_vecinos - 1] = vecino;
    return 0;
}

Nodo* crear_grafo_barabasialbert(int n, int n_inicial, int m, uint64_t *rng_state) {
    // validaciones de las entradas
    if (m > n_inicial) return NULL;
    if (n_inicial < 2) return NULL;
    if (n <= n_inicial) return NULL;

    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
        nodos[i].num_vecinos = 0;
        nodos[i].vecinos = NULL;
    }

    // conexion grafo inicial
    for (int i = 0; i < n_inicial; i++) {
        for (int j = i + 1; j < n_inicial; j++) {
            agregar_arista(&nodos[i], j);
            agregar_arista(&nodos[j], i);
        }
    }

    for (int i = n_inicial; i < n; i++) {
        int suma_grados = 0;
        for (int j = 0; j < i; j++) {
            suma_grados += nodos[j].num_vecinos;
        }

        int conectados = 0;
        int *conectado = calloc(i, sizeof(int));
        while (conectados < m) {
            int p = rand_uniform(rng_state, suma_grados);
            int acumulado = 0;

            for (int j = 0; j < i; j++) {
                if (conectado[j]) continue;
                acumulado += nodos[j].num_vecinos;
                if (p < acumulado) {
                    agregar_arista(&nodos[i], j);
                    agregar_arista(&nodos[j], i);
                    conectado[j] = 1;
                    conectados++;
                    break;
                }
            }
        }
        free(conectado);
    }
    return nodos;
}

Nodo* crear_grafo_barabasialbert_alt(int n, int n_inicial, int m_inicial, int m, uint64_t *rng_state) {
    // validaciones de las entradas
    if (m > n_inicial) return NULL;
    if (n_inicial < 2) return NULL;
    if (n <= n_inicial) return NULL;

    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) {
        return NULL;
    }
    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
        nodos[i].num_vecinos = 0;
        nodos[i].vecinos = NULL;
    }

    // conexion grafo inicial
    for (int i = 0; i < n_inicial; i++) {
        for (int j = i + 1; j < n_inicial; j++) {
            agregar_arista(&nodos[i], j);
            agregar_arista(&nodos[j], i);
        }
    }
    // usar una pura lista de aristas
    int capacidad = n_inicial * n_inicial * 2;
    int *lista_aristas = malloc(capacidad * sizeof(int));
    int num_aristas = 0;

    for (int i = 0; i < n_inicial; i++) {
        for (int j = 0; j < nodos[i].num_vecinos; j++) {
            lista_aristas[num_aristas++] = i;
        }
    }

    for (int i = n_inicial; i < n; i++) {
        int *conectado = calloc(i, sizeof(int));

        for (int k = 0; k < m; k++) {
            int indice, elegido;
            do {
                indice = rand_uniform(rng_state, num_aristas);
                elegido = lista_aristas[indice];
            } while (conectado[elegido]);

            agregar_arista(&nodos[i], elegido);
            agregar_arista(&nodos[elegido], i);
            conectado[elegido] = 1;

            lista_aristas[num_aristas++] = i;
            lista_aristas[num_aristas++] = elegido;
        }
        free(conectado);
    }
    free(lista_aristas);
    return nodos;
}

void liberar_nodos(Nodo *nodos, int n) {
    for (int i; i < n; i++) {
        free(nodos[i].vecinos);
    }
    free(nodos);
}

int revisar_consenso(uint8_t *opiniones, int n) {
    int opinion = opiniones[0];
    for (int i = 1; i < n; i++) {
        if (opinion != opiniones[i]){
            return 0;
        }
    }
    return 1;
}

// funcion para recorrer en BFS el grafo
void bfs(Nodo *nodos, int n, int *visitados) {
    // se crean los componentes
    // la cola se usa para los vecinos que se van a recorrer en la siguiente vuelta
    int *cola = malloc(n * sizeof(int));
    int frente = 0;
    int fin = 0;
    visitados[0] = 1;
    cola[fin++] = 0;

    while (frente < fin) {
        int actual = cola[frente++];
        for (int i = 0; i < nodos[actual].num_vecinos; i++) {
            int vecino = nodos[actual].vecinos[i];
            if (!visitados[vecino]) {
                visitados[vecino] = 1;
                cola[fin++] = vecino;
            }
        }
    }

    free(cola);
}
// funciona como bool con 1/0 = v/f
int Revisar_conectividad(Nodo *nodos, int n) {
    int *visitados = calloc(n, sizeof(int));
    if (!visitados) return 0;

    bfs(nodos, n, visitados);

    // se recorren los visitados para verificar conectividad
    for (int i = 0; i < n; i++) {
        if (!visitados[i]) {
            free(visitados);
            return 0;
        }
    }
    free(visitados);
    return 1;
}