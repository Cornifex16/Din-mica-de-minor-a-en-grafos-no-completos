#include "simulacion.h"
#include <time.h>
#include <stdio.h>

int main() {
    int n;
    int k;
    printf("inserte k : \n");
    scanf("%d", &k);
    printf("inserte n: \n");
    scanf("%d", &n);
    printf("antes del gossip pull \n");
    int limite = 100;
    time_t start, end;

    start = time(NULL);
    resultado res = k_uniform_gossip_pull_er(n, k, 300, limite);
    end = time(NULL);
    printf("se obtuvo el consenso: %s, en rondas: %d\n", res.consenso? "true":"false", res.rondas);
    printf("tiempo que se demoro el programa: %.2f segundos \n", difftime(end, start));
    return 0;
}