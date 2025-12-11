#ifndef SIMULACION_H
#define SIMULACION_H
#include "Grafos.h"
#include "rng.h"

uint8_t obtener_minoria(int contador_1, int k, uint64_t *rng);

void recorrer_paralelo(Nodo *nodos, int n, int k, uint8_t *opiniones, uint8_t *opiniones_siguiente, uint64_t *rng_state);

void k_uniform_gossip_pull(int n, int k, int limite);

#endif