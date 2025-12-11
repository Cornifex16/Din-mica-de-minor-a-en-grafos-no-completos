#include "simulacion.h"
#include <time.h>
#include <stdio.h>
#include <unistd.h>

int main() {
    int n;
    int k;
    int p;
    printf("inserte k : \n");
    scanf("%d", &k);
    printf("inserte n: \n");
    scanf("%d", &n);
    printf("inserte p (considere que 3 por ciento es igual a 300): \n");
    scanf("%d", &p);
    printf("antes del gossip pull \n");
    int n_cluster = n/2;
    int limite = 100;
    time_t start, end;

    // se crean los dos clusters
    uint64_t rng = time(NULL);
    Nodo *cluster_1 = crear_grafo_erdosrenyi(n_cluster, p, &rng);
    sleep(2);
    rng = time(NULL);
    Nodo *cluster_2 = crear_grafo_erdosrenyi(n_cluster, p, &rng);

    // se unen los clusters en nodos
    Nodo *nodos = malloc(n * sizeof(Nodo));
    for (int i = 0; i < n_cluster; i++) {
        nodos[i] = cluster_1[i];
    }
    for (int i = 0; i < n_cluster; i++) {
        // se hace un offset de los datos en el segundo cluster
        for (int j = 0; j < cluster_2[i].num_vecinos; j++) {
            cluster_2[i].vecinos[j] = cluster_2[i].vecinos[j] + n_cluster;
        }
        nodos[i + n_cluster] = cluster_2[i]; 
    }

    // se unen los clusters dentro de nodos en los nodos iniciales mediante una arista
    nodos[0].num_vecinos++;
    nodos[n_cluster].num_vecinos++;
    nodos[0].vecinos = realloc(nodos[0].vecinos, nodos[0].num_vecinos * sizeof(int));
    if (!nodos[0].vecinos) {
        printf("error en realloc vecinos 0");
        return 1;
    }
    nodos[n_cluster].vecinos = realloc(nodos[n_cluster].vecinos, nodos[n_cluster].num_vecinos * sizeof(int));
    if (!nodos[n_cluster].vecinos) {
        printf("error en realloc vecinos n / 2");
        return 1;
    }
    nodos[0].vecinos[nodos[0].num_vecinos - 1] = n_cluster;
    nodos[n_cluster].vecinos[nodos[n_cluster].num_vecinos - 1] = 0;
    
    // se empieza el programa principal
    start = time(NULL);
    resultado res = k_uniform_gossip_pull_alt(n, k, limite, nodos, &rng);
    end = time(NULL);
    printf("se obtuvo el consenso: %s, en rondas: %d\n", res.consenso? "true":"false", res.rondas);
    printf("tiempo que se demoro el programa: %.2f segundos \n", difftime(end, start));
    return 0;
}