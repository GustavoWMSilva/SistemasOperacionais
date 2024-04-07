#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5
#define M 10

int vez;
int flag[N];
int porcoes_na_travessa = 0;

void lock(int id) {
    int outro;
    flag[id] = 1;
    vez = id;
    for (outro = 0; outro < N; outro++) {
        if (outro != id) {
            while (flag[outro] && vez == id);
        }
    }
}

void unlock(int id) {
    flag[id] = 0;
}

void *cozinheiro(void *arg) {
    while (1) {
        if (porcoes_na_travessa == 0) {
            printf("Cozinheiro repondo a travessa...\n");
            porcoes_na_travessa = M;
        }
    }
}

void *canibal(void *arg) {
    int id = *((int *)arg);
    while (1) {
        lock(id);
        if (porcoes_na_travessa > 0) {
            porcoes_na_travessa--;
            printf("Canibal %d se serviu e comeu. Porções restantes: %d\n", id, porcoes_na_travessa);
        }
        unlock(id);
        usleep(500000); // Espera simulada para simular o tempo entre as ações dos canibais
    }
}

int main() {
    pthread_t threads_canibais[N], thread_cozinheiro;
    int ids_canibais[N];

    pthread_create(&thread_cozinheiro, NULL, cozinheiro, NULL);

    for (int i = 0; i < N; i++) {
        ids_canibais[i] = i;
        pthread_create(&threads_canibais[i], NULL, canibal, &ids_canibais[i]);
    }

    pthread_join(thread_cozinheiro, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(threads_canibais[i], NULL);
    }

    return 0;
}
