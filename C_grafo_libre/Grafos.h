#ifndef GRAFOS_H
#define GRAFOS_H
#include <stdint.h>
#include <stdlib.h>


struct Nodo{
    int opinion;
    int *vecinos;
    int num_vecinos;
} typedef Nodo;

Nodo* Crear_grafo(int n, int k, uint64_t *rng_state);

Nodo* Crear_grafo_alt(int n, int k, uint64_t *rng_state);

Nodo* crear_grafo_erdosrenyi(int n, int p, uint64_t *rng_state);

Nodo* crear_grafo_barabasialbert(int n, int n_inicial, int m, uint64_t *rng_state);

Nodo* crear_grafo_barabasialbert_alt(int n, int n_inicial, int m_inicial, int m, uint64_t *rng_state);

void simetrizar_grafo(Nodo *nodos, int n);

void liberar_nodos(Nodo *nodos, int n);

int revisar_consenso(uint8_t *opiniones, int n);

int Revisar_conectividad(Nodo *nodos, int n);

void bfs(Nodo *nodos, int n, int *visitados);

#endif