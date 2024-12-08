/*
Tâche 2.5 – Adaptez vos 3 programmes de la partie 1 pour utiliser vos primitives d’attente active.
- Mesurez la performance et intégrez là aux 3 plots produits en partie 14 avec les mêmes
contraintes qu’énoncé en Tâche 1.4.
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#define BUFFER 8
#define PROD 131072

int buffer[BUFFER];
int position_in = 0, position_out = 0;

// Variables atomiques pour gérer les places libres et pleines
atomic_int count_empty = BUFFER;
atomic_int count_full = 0;

// Spinlock pour protéger les variables `position_in` et `position_out`
pthread_spinlock_t spinlock;

void simulate_cpu_load() {
    int j = 0;
    while (j < 10000) j++;
}

// Fonction pour décrémenter une variable atomique avec attente active
void spin_wait_decrement(atomic_int* counter) {
    while (1) {
        int value = atomic_load(counter);
        if (value > 0 && atomic_compare_exchange_weak(counter, &value, value - 1)) {
            break;
        }
    }
}

// Fonction pour incrémenter une variable atomique
void spin_increment(atomic_int* counter) {
    atomic_fetch_add(counter, 1);
}

void* producteur(void* arg) {
    int id = *(int*)arg;
    for (int i = 0; i < PROD; i++) {
        spin_wait_decrement(&count_empty);  // Attente d'une place libre

        pthread_spin_lock(&spinlock);
        buffer[position_in] = id;
        position_in = (position_in + 1) % BUFFER;
        pthread_spin_unlock(&spinlock);

        spin_increment(&count_full);  // Indique qu'une donnée est prête

        simulate_cpu_load();  // Traitement en dehors de la zone critique
    }
    return NULL;
}

void* consommateur(void* arg) {
    for (int i = 0; i < PROD; i++) {
        spin_wait_decrement(&count_full);  // Attente d'une donnée

        pthread_spin_lock(&spinlock);
        position_out = (position_out + 1) % BUFFER;
        pthread_spin_unlock(&spinlock);

        spin_increment(&count_empty);  // Indique qu'une place est libre

        simulate_cpu_load();  // Traitement en dehors de la zone critique
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nb_producteurs> <nb_consommateurs>\n", argv[0]);
        return 1;
    }

    int nb_producteurs = atoi(argv[1]);
    int nb_consommateurs = atoi(argv[2]);

    pthread_t producteurs[nb_producteurs], consommateurs[nb_consommateurs];
    int ids[nb_producteurs];

    pthread_spin_init(&spinlock, 0);  // Initialisation du spinlock

    // Création des threads producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        ids[i] = i;
        pthread_create(&producteurs[i], NULL, producteur, &ids[i]);
    }

    // Création des threads consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, NULL);
    }

    // Attente de la fin des threads producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        pthread_join(producteurs[i], NULL);
    }

    // Attente de la fin des threads consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_join(consommateurs[i], NULL);
    }

    // Destruction du spinlock
    pthread_spin_destroy(&spinlock);

    return 0;
}
