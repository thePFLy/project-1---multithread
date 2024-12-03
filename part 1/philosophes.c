/*
Tâche 1.1 – Coder le problème des philosophes :
- Le nombre de philosophes N est un paramètre obtenu à partir de la ligne de commande ;
- Chaque philosophe effectue 1,000,000 cycles penser/manger ;
- On n’utilise pas d’attente dans les phases manger et penser (ces actions sont immédiates)
pour mettre en avant le coût des opérations de synchronisation.
*/

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CYCLES 1000000

pthread_mutex_t *baguettes;
int N;

void* philosophe (void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;

    for (int i = 0; i < CYCLES; i++) {
        // Ordre croissant -> éviter deadlock
        if (left < right) {
            pthread_mutex_lock(&baguettes[left]);
            pthread_mutex_lock(&baguettes[right]);
        } else {
            pthread_mutex_lock(&baguettes[right]);
            pthread_mutex_lock(&baguettes[left]);
        }

        // Manger
        pthread_mutex_unlock(&baguettes[left]);
        pthread_mutex_unlock(&baguettes[right]);
        // Penser
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nombre_de_philosophes>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    if (N <= 1) {
        fprintf(stderr, "Le nombre de philosophes doit être supérieur à 1.\n");
        return 1;
    }

    pthread_t philosophes[N];
    int ids[N];
    baguettes = malloc(N * sizeof(pthread_mutex_t));

    for (int i = 0; i < N; i++) {
        pthread_mutex_init(&baguettes[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophes[i], NULL, philosophe, &ids[i]);
    }

    for (int i = 0; i < N; i++) {
        pthread_join(philosophes[i], NULL);
    }

    for (int i = 0; i < N; i++) {
        pthread_mutex_destroy(&baguettes[i]);
    }

    free(baguettes);
    return 0;
}