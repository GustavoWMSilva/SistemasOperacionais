#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

int N;
int M;
int portions_in_bowl = 0;
int turn = 0;
int *flag;

sem_t semaphore_full, semaphore_empty;

void lock(int id) {
    int other;
    flag[id] = 1;
    turn = id;
    for (other = 0; other < N; other++) {
        if (other != id) {
            while (flag[other] && turn == id);
        }
    }
}

void unlock(int id) {
    flag[id] = 0;
}

void *chef(void *arg) {
    while (1) {
        // Aguarda até a travessa estar vazia
        sem_wait(&semaphore_empty);
        
        // Cozinha e reabastece a travessa
        printf("Cozinheiro repondo a travessa...\n");
        portions_in_bowl = M;
        
        // Sinaliza que a travessa foi reabastecida
        sem_post(&semaphore_full);
    }
}

void *canibal(void *arg) {
    int id = *((int *)arg);
    while (1) {
        // Espera haver porções disponíveis
        sem_wait(&semaphore_full);
        
        // Se serve e come
        lock(id); // Inicia seção crítica
        portions_in_bowl--;
        printf("Canibal %d se serviu e comeu. Porções restantes: %d\n", id, portions_in_bowl);
        
        // Verifica se a travessa está vazia
        if (portions_in_bowl == 0) {
            // Se a travessa está vazia, sinaliza para o chef
            sem_post(&semaphore_empty);
        } else {
            // Caso contrário, sinaliza para liberar a próxima operação
            sem_post(&semaphore_full);
        }
        
        unlock(id); // Encerra seção crítica
        
        // Espera simulada para simular o tempo entre as ações dos canibais
        usleep(500000);
    }
}

int main(int argc, char *argv[]) {
    N = atoi(arg[1]); // Número de canibais
    M = atoi(arg[2]); // Porções por reabastecimento
    
    // Inicializa o vetor flag e os semáforos
    flag = calloc(N, sizeof(int));
    sem_init(&semaphore_full, 0, 0);
    sem_init(&semaphore_empty, 0, 1);
    
    // Cria as threads do chef e dos canibais
    pthread_t thread_chef, threads_canibais[N];
    int ids_canibais[N];
    
    pthread_create(&thread_chef, NULL, chef, NULL);
    
    for (int i = 0; i < N; i++) {
        ids_canibais[i] = i;
        pthread_create(&threads_canibais[i], NULL, canibal, &ids_canibais[i]);
    }
    
    // Aguarda as threads terminarem
    pthread_join(thread_chef, NULL);
    
    for (int i = 0; i < N; i++) {
        pthread_join(threads_canibais[i], NULL);
    }
    
    // Libera os recursos usados pelos semáforos e o vetor flag
    sem_destroy(&semaphore_full);
    sem_destroy(&semaphore_empty);
    free(flag);
    
    return 0;
}
