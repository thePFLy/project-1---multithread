#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    volatile int value; // Valeur du sémaphore (compteur)
} semaphore_t;

// Initialisation du sémaphore
void semaphore_init(semaphore_t *sem, int value) {
    sem->value = value; // Initialiser le compteur du sémaphore
}

// Attente active (P operation) : décrémente le compteur ou attend
void semaphore_wait(semaphore_t *sem) {
    while (1) {
        // Utilisation d'une opération atomique pour vérifier si la ressource est disponible
        if (__sync_fetch_and_sub(&sem->value, 1) > 0) {
            break; // La ressource est disponible, on entre dans la section critique
        } else {
            // Si le sémaphore est à 0, on doit attendre activement
            __sync_fetch_and_add(&sem->value, 1); // Restaurer le sémaphore à 1 si on était en attente
        }
    }
}

// Libération du sémaphore (V operation) : incrémente le compteur
void semaphore_signal(semaphore_t *sem) {
    __sync_fetch_and_add(&sem->value, 1); // Incrémente atomiquement la valeur du sémaphore
}

// Fonction simulant une section critique
void critical_section(int id) {
    printf("Thread %d dans la section critique\n", id);
    // Simulation d'un travail en section critique
    for (int i = 0; i < 10000; i++) {
        asm volatile("" ::: "memory"); // Simulation de calcul
    }
    printf("Thread %d quitte la section critique\n", id);
}

// Fonction exécutée par chaque thread
void *thread_func(void *arg) {
    int id = *((int *)arg);
    static semaphore_t sem; // Déclaration statique pour la synchronisation entre threads
    static int initialized = 0;

    if (!initialized) {
        semaphore_init(&sem, 1); // Initialiser le sémaphore avec 1 (binaire)
        initialized = 1;
    }

    // Attente active pour entrer dans la section critique
    semaphore_wait(&sem);
    critical_section(id);  // Exécution de la section critique
    semaphore_signal(&sem); // Libération du sémaphore

    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nombre_de_threads>\n", argv[0]);
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Le nombre de threads doit être supérieur à 0.\n");
        return 1;
    }

    pthread_t threads[num_threads];
    int ids[num_threads];

    // Créer et exécuter les threads
    for (int i = 0; i < num_threads; i++) {
        ids[i] = i;
        pthread_create(&threads[i], NULL, thread_func, &ids[i]);
    }

    // Attendre la fin des threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
