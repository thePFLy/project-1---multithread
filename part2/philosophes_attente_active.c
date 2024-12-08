#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define CYCLES 1000000

pthread_spinlock_t *baguettes;
int N;

void* philosophe(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;

    for (int i = 0; i < CYCLES; i++) {
        // Ordre croissant -> éviter deadlock
        if (left < right) {
            pthread_spin_lock(&baguettes[left]);
            pthread_spin_lock(&baguettes[right]);
        } else {
            pthread_spin_lock(&baguettes[right]);
            pthread_spin_lock(&baguettes[left]);
        }

        // Manger
        pthread_spin_unlock(&baguettes[left]);
        pthread_spin_unlock(&baguettes[right]);
        // Penser
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    // arguments
    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <nombre_de_philosophes>\n", argv[0]);
        return 1;
    }
    N = atoi(argv[1]);
    if (N <= 1) {
        fprintf(stderr, "Nombre de philosophes doit être > 1.\n");
        return 1;
    }

    // tableau threads pour philosophes et tableau d'id
    pthread_t philosophes[N];
    int ids[N];
    baguettes = malloc(N * sizeof(pthread_spinlock_t));

    // spinlocks pour les baguettes
    for (int i = 0; i < N; i++) {
        pthread_spin_init(&baguettes[i], 0);
    }

    // création thread pour philosophes
    for (int i = 0; i < N; i++) {
        ids[i] = i;
        pthread_create(&philosophes[i], NULL, philosophe, &ids[i]);
    }

    // Attente de la fin des threads
    for (int i = 0; i < N; i++) {
        pthread_join(philosophes[i], NULL);
    }

    // Destruction des spinlocks
    for (int i = 0; i < N; i++) {
        pthread_spin_destroy(&baguettes[i]);
    }

    // Libération de la mémoire allouée
    free(baguettes);

    return 0;
}
