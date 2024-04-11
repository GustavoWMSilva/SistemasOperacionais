#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>


/*
  declaração de veriáveis globais
  e estabelecido que a travessa de comida dos canibais começa com 0 porções
*/
int N;
int M;
int turn;
int *flag;
int portions_in_bowl = 0;


/*
  Criada as primitivas do lock() e unlock() como requerida no caso
  de uso de semaforos binário (mutex)
*/
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


/*
  função que repôem as porções na travessa
  acordando o cozinheiro apenas quando não há mais porções na travessa
*/
void *chef(void *arg) {
    while (1) {
        if (portions_in_bowl == 0) {
            printf("Cozinheiro repondo a travessa...\n");
            portions_in_bowl = M;  // enche a travessa com o valor passado no argumento
        }
    }
}

void *canibal(void *arg) {
    int id = *((int *)arg);
    while (1) {
        lock(id);  // Seção Crítica iniciada
        if (portions_in_bowl > 0) {  // se há porções na travessa
            portions_in_bowl--;  // decrementa 1 porção da travessa
            printf("Canibal %d se serviu e comeu. Porções restantes: %d\n", id, portions_in_bowl);
        }
        unlock(id);  // Seção Crítica encerrada
        usleep(500000);  // Espera simulada para simular o tempo entre as ações dos canibais
    }
}

int main(int n, char* arg[]){
    
    N = atoi(arg[1]);  // primeiro argumento na linha de comando
    M = atoi(arg[2]);  // segundo argumento na linha de comando

    flag = calloc(N, sizeof(int));  // declara o tamanho da flag

    // declara que essas variáveis são do tipo threads
    pthread_t threads_canibais[N], thread_chef;
    
    int ids_canibais[N];

    /*
      pthread_create: Cria a thread e no caso de um atribulo ser NULL
      é utilizado o atributo padrão
    */
    pthread_create(&thread_chef, NULL, chef, NULL);

    for (int i = 0; i < N; i++) {
        ids_canibais[i] = i;
        pthread_create(&threads_canibais[i], NULL, canibal, &ids_canibais[i]);
    }

    // pthread_join: que a thread chamada aguarde o término da thread de destino.
    pthread_join(thread_chef, NULL);

    for (int i = 0; i < N; i++) {
        pthread_join(threads_canibais[i], NULL);
    }

    return 0;
}
