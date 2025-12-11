#ifndef GRAFOS_ALT_H
#define GRAFOS_ALT_H
#include <stdint.h>
#include <stdlib.h>

struct Nodo{
    int opinion;
    int *vecinos;
    int num_vecinos;
} typedef Nodo;

Nodo* crear_grafo_erdosrenyi(int n, int p, uint64_t *rng_state);

Nodo* crear_grafo_barabasialbert(int n, int n_inicial, int m_inicial, int m, uint64_t *rng_state);

Nodo* crear_grafo_barabasialbert_alt(int n, int n_inicial, int m_inicial, int m, uint64_t *rng_state);

void bfs(Nodo *nodos, int n, int *visitados);

int Revisar_conectividad(Nodo *nodos, int n);

#endif