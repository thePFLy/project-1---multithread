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

void* philosophe(void *arg) {
    int id = *(int *)arg;
    int left = id;
    int right = (id + 1) % N;

    int i = 0;
    while (i < CYCLES) {
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
        i++;
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

    // allocation mémoire pour les baguettes
    baguettes = malloc(N * sizeof(pthread_mutex_t));

    // mutex pour les baguettes
    int j = 0;
    while (j < N) {
        pthread_mutex_init(&baguettes[j], NULL);
        j++;
    }

    // création thread pour philosophes
    for (int i = N - 1; i >= 0; i--) {
        ids[i] = i;
        pthread_create(&philosophes[i], NULL, philosophe, &ids[i]);
    }

    // fin thread philosophes
    int k = 0;
    while (k < N) {
        pthread_join(philosophes[k], NULL);
        k++;
    }

    // "destruction" mutex
    for (int i = N - 1; i >= 0; i--) {
        pthread_mutex_destroy(&baguettes[i]);
    }

    // libere mémoire allouée
    free(baguettes);
    return 0;
}
