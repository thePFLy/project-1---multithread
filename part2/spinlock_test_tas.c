#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

// Paramètres pour les threads
typedef struct {
    int id;
    int sections;
} thread_params_t;

// Verrou global
volatile int lock = 0;

// Fonction pour acquérir le verrou
void lock_tas() {
    int expected;
    do {
        expected = 1;
        asm volatile (
            "xchgl %0, %1"
            : "=r" (expected), "=m" (lock) // Sorties
            : "0" (expected)              // Entrée (%eax initialisé à 1)
            : "memory"
        );
    } while (expected != 0); // Boucle tant que le verrou n'est pas libre
}

// Fonction pour libérer le verrou
void unlock_tas() {
    int value = 0;
    asm volatile (
        "xchgl %0, %1"
        : "=r" (value), "=m" (lock) // Sorties
        : "0" (value)              // Entrée (%eax initialisé à 0)
        : "memory"
    );
}

// Section critique
void critical_section() {
    for (int i = 0; i < 10000; i++) {
        asm volatile("" ::: "memory");
    }
}

// Fonction exécutée par chaque thread
void *thread_func(void *arg) {
    thread_params_t *params = (thread_params_t *)arg;
    int sections = params->sections;

    for (int i = 0; i < sections; i++) {
        lock_tas(); // Acquérir le verrou
        critical_section(); // Section critique
        unlock_tas(); // Libérer le verrou
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <nombre_de_threads>\n", argv[0]); // Affichage erreur sur sortie STDERR
        return 1;
    }

    int num_threads = atoi(argv[1]);
    if (num_threads <= 0) {
        fprintf(stderr, "Le nombre de threads doit être supérieur à 0.\n"); // Affichage erreur sur sortie STDERR
        return 1;
    }

    // Calcul du nombre de sections critiques par thread
    int total_sections = 32768;
    int sections_per_thread = total_sections / num_threads;

    // Initialisation des threads
    pthread_t threads[num_threads];
    thread_params_t params[num_threads];

    // Lancement des threads
    for (int i = 0; i < num_threads; i++) {
        params[i].id = i;
        params[i].sections = sections_per_thread;
        pthread_create(&threads[i], NULL, thread_func, &params[i]);
    }

    // Attendre la fin des threads
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}
