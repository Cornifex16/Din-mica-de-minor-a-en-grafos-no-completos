#ifndef SIMULACION_H
#define SIMULACION_H
#include "Grafos.h"
#include "rng.h"

struct resultado
{
    int rondas;
    int consenso;
} typedef resultado;


uint8_t obtener_minoria(int contador_1, int k, uint64_t *rng);

void recorrer_paralelo(Nodo *nodos, int n, int k, uint8_t *opiniones, uint8_t *opiniones_siguiente, uint64_t *rng_state);

resultado k_uniform_gossip_pull(int n, int k, int limite);
resultado k_uniform_gossip_pull_alt(int n, int k, int limite, Nodo *nodos, uint64_t *rng);
resultado k_uniform_gossip_pull_er(int n, int k, int p, int limite);
resultado k_uniform_gossip_pull_er_kalt(int n, int p, float a, int limite);

#endif