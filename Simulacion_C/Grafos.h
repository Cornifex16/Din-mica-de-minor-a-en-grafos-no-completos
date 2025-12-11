#ifndef GRAFOS_H
#define GRAFOS_H
#include <stdint.h>


struct Nodo{
    int opinion;
    uint8_t *opiniones_vecinos; 
} typedef Nodo;

Nodo* crear_nodos(int n, uint64_t *rng_state);

void liberar_nodos(Nodo *nodos);

int revisar_consenso(uint8_t *opiniones, int n);

#endif