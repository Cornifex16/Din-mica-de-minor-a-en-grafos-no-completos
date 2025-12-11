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
        int contador_1 = 0;
        for (int t = 0; t < k; t++) {
            int j = rand_uniform(rng_state, n);
            while (j == i) j = rand_uniform(rng_state, n);
            contador_1 += opiniones[j];
        }
        opiniones_siguiente[i] = obtener_minoria(contador_1, k, rng_state);
    }
}

void k_uniform_gossip_pull(int n, int k, int limite) {
    uint64_t rng = time(NULL);

    Nodo *nodos = crear_nodos(n, &rng);
    uint8_t *opiniones = malloc(n * sizeof(uint8_t));
    uint8_t *opiniones_siguiente = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++) {
        opiniones[i] = nodos[i].opinion;
    }

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
    printf("N=%d, rondas=%d, consenso=%s\n", n,
        contador, consenso ? "SI" : "NO");
    free(opiniones);
    free(opiniones_siguiente);
    liberar_nodos(nodos);
}