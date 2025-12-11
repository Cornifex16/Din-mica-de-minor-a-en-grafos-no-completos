#include "simulacion.h"
#include <time.h>
#include <stdio.h>

int main() {
    int n = 100;
    int k = 50;
/*     printf("inserte k: \n");
    scanf("%d", &k);
    printf("inserte n: \n");
    scanf("%d", &n); */
    int limite = 100;
    time_t start, end;

    start = time(NULL);
    k_uniform_gossip_pull(n, k, limite);
    end = time(NULL);
    printf("tiempo que se demoro el programa: %.2f segundos \n", difftime(end, start));
    return 0;
}