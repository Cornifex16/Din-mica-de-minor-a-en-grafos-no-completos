#include "simulacion.h"
#include "Grafos.h"
#include <stdio.h>
#include <time.h>
#include <math.h>


void test_2() {
    int n = 400;
    int k;
    int limite = 100;
    int s = 100;
    FILE *stream;
    stream = fopen("datos_n400_er", "w");
    fprintf(stream, "k,rondas,consenso\n");
    for (k=58; k < n/2; k++){
        for (int i = 0; i < s; i++) {
            resultado res;
            res = k_uniform_gossip_pull(n,k,limite);
            fprintf(stream, "%d,%d,%s\n", k, res.rondas, res.consenso? "True":"False");
        }
    }
    fclose(stream);
}

void test_er() {
    int n = 400;
    int k;
    int p;
    int limite = 100;
    int s = 100;
    FILE *stream;
    stream = fopen("datos_n400_er3d", "w");
    fprintf(stream, "k,p,rondas,consenso\n");
    for (k=58; k < n/2; k++){
        for (p=300; p < 400; p++) {
            for (int i = 0; i < s; i++) {
                resultado res;
                res = k_uniform_gossip_pull_er(n,k,p,limite);
                fprintf(stream, "%d,%d,%d,%s\n", k, p, res.rondas, res.consenso? "True":"False");
            }
        }
    }
    fclose(stream);
}

void test_er_nvar3d() {
    int n;
    int k;
    int p;
    int limite = 100;
    int s = 100;
    FILE *stream;
    stream = fopen("datos_nvar_er3d", "w");
    fprintf(stream, "n,p,rondas,consenso\n");
    for (n=400; n < 2000; n = n * 2){
        for (p=10000 * ((log2(n)/n)); p < 400; p++) {
            for (int i = 0; i < s; i++) {
                resultado res;
                k = n/2;
                res = k_uniform_gossip_pull_er(n,k,p,limite);
                fprintf(stream, "%d,%d,%d,%s\n", n, p, res.rondas, res.consenso? "True":"False");
            }
        }
    }
    fclose(stream);
}

void test_er_nvar() {
    int n;
    int k;
    int p;
    int limite = 100;
    int s = 1000;
    FILE *stream;
    stream = fopen("datos_nvar_er_ksup", "w");
    fprintf(stream, "n,rondas,consenso\n");
    for (n=400; n < 50000; n = n * 2){
        for (int i = 0; i < s; i++) {
            resultado res;
            k = sqrt(n * log2(n));
            p = 10000 * ((log2(n)/n));
            res = k_uniform_gossip_pull_er(n,k,p,limite);
            fprintf(stream, "%d,%d,%s\n", n, res.rondas, res.consenso? "True":"False");
        }
    }
    fclose(stream);
}

void test_er_alt() {
    int n = 400;
    int k;
    int p;
    int limite = 100;
    int s = 5000;
    FILE *stream;
    stream = fopen("datos_n400_er_kalt", "w");
    fprintf(stream, "p,rondas,consenso\n");
    for (p=220; p < 400; p++) {
        for (int i = 0; i < s; i++) {
            resultado res;
            res = k_uniform_gossip_pull_er_kalt(n,p,0.5,limite);
            fprintf(stream, "%d,%d,%s\n", p, res.rondas, res.consenso? "True":"False");
        }
    }
    fclose(stream);
}

void test_er_alpha() {
    int n;
    float a;
    int p;
    int limite = 100;
    int s = 1000;
    FILE *stream;
    stream = fopen("datos_nvar_er_alpha", "w");
    fprintf(stream, "n,rondas,consenso\n");
    for (n=100; n < 1000000; n =  n*2) {
        for (a=0.1; a < 1.0; a = a + 0.1) {
            for (int i = 0; i < s; i++) {
                resultado res;
                int p = 10000 * ((log2(n)/n));
                printf("%d", p);
                res = k_uniform_gossip_pull_er_kalt(n,p,a,limite);
                fprintf(stream, "%d,%f,%d,%s\n", n, a, res.rondas, res.consenso? "True":"False");
            }
        }
    }
    fclose(stream);
}

void test_er_alpha_2() {
    int n;
    float a;
    int p;
    int limite = 100;
    int s = 1000;
    FILE *stream;
    stream = fopen("datos_nvar_er_alpha_2", "w");
    fprintf(stream, "n,rondas,consenso\n");
    for (n=51200; n < 1000000; n =  n*2) {
        for (a=0.9; a < 1.0; a = a + 0.1) {
            for (int i = 0; i < s; i++) {
                resultado res;
                int p = 10000 * ((log2(n)/n));
                printf("%d", p);
                res = k_uniform_gossip_pull_er_kalt(n,p,a,limite);
                fprintf(stream, "%d,%f,%d,%s\n", n, a, res.rondas, res.consenso? "True":"False");
            }
        }
    }
    fclose(stream);
}

void test_er_nalt() {
    int n;
    int k;
    int limite = 100;
    int s = 1000;
    FILE *stream;
    stream = fopen("datos_nvar_er_kalt", "w");
    fprintf(stream, "n,rondas,consenso\n");
    for (n=100; n < 1000000; n =  n*2) {
        for (int i = 0; i < s; i++) {
            resultado res;
            int p = 10000 * ((log2(n)/n));
            printf("%d", p);
            res = k_uniform_gossip_pull_er_kalt(n,p,0.5,limite);
            fprintf(stream, "%d,%d,%s\n", n, res.rondas, res.consenso? "True":"False");
        }
    }
    fclose(stream);
}

void test_1() {
    int n = 400;
    int k = 100;
    int limite = 100;
    int s = 100;
    uint64_t rng = time(NULL);
    Nodo *nodos = Crear_grafo_alt(n, k, &rng);
    // opiniones iniciales
    uint8_t *opiniones_iniciales = malloc(n * sizeof(uint8_t));
    for (int i = 0; i < n; i++)
        opiniones_iniciales[i] = nodos[i].opinion;
    int indice_aleatorio = rand_uniform(&rng, n);
    FILE *stream;
    FILE *stream2;
    stream = fopen("datos_n400_fijo", "w");
    stream2 = fopen("ejemplo_constancia.txt", "w");

    if (!stream || !stream2) {
        perror("Error al abrir archivo");
        return ;
    }
    fprintf(stream, "k,rondas,consenso\n");
    fprintf(stream2, "indice: %d, nodo valor: %d\n", indice_aleatorio, nodos[indice_aleatorio].opinion);
    for (k = 58; k < n/2; k++) {
        for (int i=0; i < s; i++) {
            // reinicia las opiniones a su valor inicial
            for (int j = 0; j < n; j++) {
                nodos[j].opinion = opiniones_iniciales[j];
            }
            rng = time(NULL);
            fprintf(stream2, "indice: %d, nodo valor: %d\n", indice_aleatorio, nodos[indice_aleatorio].opinion);
            resultado res;
            res = k_uniform_gossip_pull_alt(n, k, limite, nodos, &rng);
            fprintf(stream, "%d,%d,%s\n", k, res.rondas, res.consenso? "True":"False");
        }
    }
    fclose(stream);
    fclose(stream2);
    free(opiniones_iniciales);
    liberar_nodos(nodos, n);
}

void test_random() {
    uint64_t rng = time(NULL);
    for (int i = 0; i < 100; i++) {
        printf("%d \n", rand_uniform(&rng, 100));
    }
}

void trash_test() {
    int a = 10;
    float b = 0.5;
    int c = a * b;
    printf("%d \n", c);
}

void main() {
    test_er_nvar();
}