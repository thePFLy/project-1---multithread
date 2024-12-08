#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

typedef struct {
    volatile int value; // Valeur du sémaphore (compteur)
} semaphore_t;

// Initialisation du sémaphore
void semaphore_init(semaphore_t *sem, int value) {
    sem->value = value; // Initialisation du compteur du sémaphore
}

// Attente active
void semaphore_wait(semaphore_t *sem) {
    while (1) {
        // Utilisation d'une opération atomique pour vérifier si la ressource est disponible
        if (__sync_fetch_and_sub(&sem->value, 1) > 0) {
            break;
        } else {
            // Si le sémaphore est à 0, on doit attendre activement
            __sync_fetch_and_add(&sem->value, 1);
        }
    }
}

// Libération du sémaphore
void semaphore_signal(semaphore_t *sem) {
    __sync_fetch_and_add(&sem->value, 1); // Incrémente atomiquement la valeur du sémaphore
}

// Fonction simulant une section critique
void critical_section() {
    for (int i = 0; i < 10000; i++) {
        asm volatile("" ::: "memory");
    }
}

// Fonction exécutée par chaque thread
void *thread_func(void *arg) {
    static semaphore_t sem;
    static int initialized = 0;

    if (!initialized) {
        semaphore_init(&sem, 1); // Initialisation du sémaphore avec 1
        initialized = 1;
    }

    semaphore_wait(&sem); // Attente active pour entrer dans la section critique
    critical_section(); // Exécution de la section critique
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

    // Création et exécution des threads
    for (int i = 0; i < num_threads; i++) {
        pthread_create(&threads[i], NULL, thread_func, NULL);
    }

    // Attendre la fin des threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
