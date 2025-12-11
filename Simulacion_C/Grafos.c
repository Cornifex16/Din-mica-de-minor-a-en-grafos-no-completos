#include <stdio.h>
#include <stdlib.h>
#include "Grafos.h"


static inline uint64_t splitmix64(uint64_t *x) {
    uint64_t z = (*x += 0x9e3779b97f4a7c15ULL);
    z = (z ^ (z >> 30)) * 0xbf58476d1ce4e5b9ULL;
    z = (z ^ (z >> 27)) * 0x94d049bb133111ebULL;
    return z ^ (z >> 31);
}

Nodo* crear_nodos(int n, uint64_t *rng_state) {
    Nodo *nodos = malloc(n * sizeof(Nodo));
    if (!nodos) {
        return NULL;
    }

    for (int i = 0; i < n; i++) {
        nodos[i].opinion = (splitmix64(rng_state) & 1);
    }
    return nodos;
}

void liberar_nodos(Nodo *nodos) {
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