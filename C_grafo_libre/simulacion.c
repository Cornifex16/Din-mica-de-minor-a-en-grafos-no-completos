#include "simulacion.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


uint8_t obtener_minoria(int contador_1, int k, uint64_t *rng) {
    int contador_0 = k - contador_1;
    if(contador_1 < contador_0) return 1;
    else if (contador_0 < contador_1) return 0;
    else return (splitmix64(rng) & 1);
}

void recorrer_paralelo(Nodo *nodos, int n, int k, uint8_t *opiniones, uint8_t *opiniones_siguiente, uint64_t *rng_state) {
    for (int i=0; i < n; i++) {
        Nodo *v = &nodos[i];
        int k_act = 0;
        k_act = k;
        if (v->num_vecinos < k) k_act = v->num_vecinos;
        int contador_1 = 0;
        int *usados = malloc(k * sizeof(int));
        if (!usados) { 
            fprintf(stderr, "Error: sobrepaso de memoria. \n"); 
            exit(1);
        }
        int usados_contador = 0;

        for (int t = 0; t < k_act; t++) {
            int indice;
            int repetido;
            do
            {
                indice = rand_uniform(rng_state, v->num_vecinos);
                repetido = 0;
                for (int u = 0; u < usados_contador && !repetido; u++) {
                    if (usados[u] == indice) {
                        repetido = 1;
                    }
                }
            } while (repetido);
            usados[usados_contador++] = indice;
            
            int vecino = v->vecinos[indice];
            contador_1 += opiniones[vecino];
        }
        opiniones_siguiente[i] = obtener_minoria(contador_1, k_act, rng_state);
        free(usados);
    }
}

void recorrer_paralelo_kalt(Nodo *nodos, int n, float a, uint8_t *opiniones, uint8_t *opiniones_siguiente, uint64_t *rng_state) {
    for (int i=0; i < n; i++) {
        Nodo *v = &nodos[i];
        if (v->num_vecinos == 0) {
            printf("hay un nodo desconectado");
        }
        int k = v->num_vecinos * a;
        if (k == 0) k = 1;
        int contador_1 = 0;
        int *usados = malloc(k * sizeof(int));
        if (!usados) { 
            fprintf(stderr, "Error: sobrepaso de memoria. \n"); 
            exit(1);
        }
        int usados_contador = 0;

        for (int t = 0; t < k; t++) {
            int indice;
            int repetido;
            do
            {
                indice = rand_uniform(rng_state, v->num_vecinos);
                repetido = 0;
                for (int u = 0; u < usados_contador && !repetido; u++) {
                    if (usados[u] == indice) {
                        repetido = 1;
                    }
                }
            } while (repetido);
            usados[usados_contador++] = indice;
            
            int vecino = v->vecinos[indice];
            contador_1 += opiniones[vecino];
        }
        opiniones_siguiente[i] = obtener_minoria(contador_1, k, rng_state);
        free(usados);
    }
}

resultado k_uniform_gossip_pull(int n, int k, int limite) {
    uint64_t rng = time(NULL);

    Nodo *nodos = crear_grafo_erdosrenyi(n, 20, &rng);
    printf("se creo el grafo");
    uint8_t *opiniones = malloc(n * sizeof(uint8_t));
    uint8_t *opiniones_siguiente = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++) {
        opiniones[i] = nodos[i].opinion;
    }
    int indice_aleatorio = rand_uniform(&rng, n);
    Nodo nodo_aleatorio = nodos[indice_aleatorio];
    int consenso = 0;
    int contador = 0;

    while (!consenso && contador < limite) {
        recorrer_paralelo(nodos, n, k, opiniones, opiniones_siguiente, &rng);
        for (int i = 0; i < n; i++) {
            opiniones[i] = opiniones_siguiente[i];
            nodos[i].opinion = opiniones_siguiente[i];

        }
        consenso = revisar_consenso(opiniones, n);
        contador++;
    }
    free(opiniones);
    free(opiniones_siguiente);
    liberar_nodos(nodos, n);
    resultado res;
    res.consenso = consenso;
    res.rondas = contador;
    return res;
}

resultado k_uniform_gossip_pull_er(int n, int k, int p, int limite) {
    uint64_t rng = time(NULL);

    Nodo *nodos = crear_grafo_erdosrenyi(n, p, &rng);
    uint8_t *opiniones = malloc(n * sizeof(uint8_t));
    uint8_t *opiniones_siguiente = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++) {
        opiniones[i] = nodos[i].opinion;
    }
    int indice_aleatorio = rand_uniform(&rng, n);
    Nodo nodo_aleatorio = nodos[indice_aleatorio];
    int consenso = 0;
    int contador = 0;


    while (!consenso && contador < limite) {
        recorrer_paralelo(nodos, n, k, opiniones, opiniones_siguiente, &rng);
        for (int i = 0; i < n; i++) {
            opiniones[i] = opiniones_siguiente[i];
            nodos[i].opinion = opiniones_siguiente[i];

        }
        consenso = revisar_consenso(opiniones, n);
        contador++;
    }
    free(opiniones);
    free(opiniones_siguiente);
    liberar_nodos(nodos, n);
    resultado res;
    res.consenso = consenso;
    res.rondas = contador;
    return res;
}

resultado k_uniform_gossip_pull_er_kalt(int n, int p, float a, int limite) {
    uint64_t rng = time(NULL);

    Nodo *nodos = crear_grafo_erdosrenyi(n, p, &rng);
    uint8_t *opiniones = malloc(n * sizeof(uint8_t));
    uint8_t *opiniones_siguiente = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++) {
        opiniones[i] = nodos[i].opinion;
    }
    int indice_aleatorio = rand_uniform(&rng, n);
    Nodo nodo_aleatorio = nodos[indice_aleatorio];
    int consenso = 0;
    int contador = 0;

    while (!consenso && contador < limite) {
        recorrer_paralelo_kalt(nodos, n, a, opiniones, opiniones_siguiente, &rng);
        for (int i = 0; i < n; i++) {
            opiniones[i] = opiniones_siguiente[i];
            nodos[i].opinion = opiniones_siguiente[i];

        }
        consenso = revisar_consenso(opiniones, n);
        contador++;
    }
    free(opiniones);
    free(opiniones_siguiente);
    liberar_nodos(nodos, n);
    resultado res;
    res.consenso = consenso;
    res.rondas = contador;
    return res;
}

resultado k_uniform_gossip_pull_alt(int n, int k, int limite, Nodo *nodos, uint64_t *rng) {
    uint8_t *opiniones = malloc(n * sizeof(uint8_t));
    uint8_t *opiniones_siguiente = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++) {
        opiniones[i] = nodos[i].opinion;
    }
    int consenso = 0;
    int contador = 0;

    while (!consenso && contador < limite) {
        recorrer_paralelo(nodos, n, k, opiniones, opiniones_siguiente, rng);
        for (int i = 0; i < n; i++) {
            opiniones[i] = opiniones_siguiente[i];
            nodos[i].opinion = opiniones_siguiente[i];

        }
        consenso = revisar_consenso(opiniones, n);
        contador++;
    }
    free(opiniones);
    free(opiniones_siguiente);
    resultado res;
    res.consenso = consenso;
    res.rondas = contador;
    return res;
}