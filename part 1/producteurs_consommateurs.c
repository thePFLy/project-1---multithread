/*
Tâche 1.2 – Coder le problème des producteurs-consommateurs :
- Le nombre de threads consommateur et le nombre de threads producteurs sont deux
paramètres obtenus à partir de la ligne de commande ;
- Le buffer est un tableau partagé de 8 places, contenant des entiers (int) :
o Une donnée produite ne doit jamais ‘écraser’ une donnée non consommée !
o À chaque insertion, chaque thread insère dans le buffer un int fixe, qui est son
identifiant.
- Entre chaque consommation ou PROD, un thread « simule » un traitement utilisant de
la ressource CPU, en utilisant : for (int i=0; i<10000; i++);
- Le traitement se fait en dehors de la zone critique.
- Le nombre d’éléments produits (et donc consommé) est toujours de 131072.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define BUFFER 8
#define PROD 131072

int buffer[BUFFER];
int position_in = 0, position_out = 0;
sem_t sem_empty, sem_full;
pthread_mutex_t mtx;

void simulate_cpu_load() {
    int j = 0;
    while (j < 10000) j++;
}

// Pour chaque producteur
void* producteur(void* arg) {
    int id = *(int*)arg;
    int i = 0;
    while (i < PROD) {
        sem_wait(&sem_empty); 
        pthread_mutex_lock(&mtx);

        buffer[position_in] = id;
        position_in = (position_in + 1) % BUFFER;

        pthread_mutex_unlock(&mtx);
        sem_post(&sem_full); 

        simulate_cpu_load();
        i++;
    }
    return NULL;
}

void* consommateur(void* arg) {
    int i = 0;
    while (i < PROD) {
        sem_wait(&sem_full);
        pthread_mutex_lock(&mtx);

        int item = buffer[position_out];
        position_out = (position_out + 1) % BUFFER;

        pthread_mutex_unlock(&mtx);
        sem_post(&sem_empty);
        // zone crit
        simulate_cpu_load();
        i++;
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    // 2 arguments exactement
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nb_producteurs> <nb_consommateurs>\n", argv[0]);
        return 1;
    }
    // nbr de producteur et consommateur (grace aux arguments)
    int nb_producteurs = atoi(argv[1]);
    int nb_consommateurs = atoi(argv[2]);

    // prépare threads
    pthread_t producteurs[nb_producteurs], consommateurs[nb_consommateurs];
    int ids[nb_producteurs];

    // buffer
    sem_init(&sem_empty, 0, BUFFER);  
    sem_init(&sem_full, 0, 0);
    pthread_mutex_init(&mtx, NULL);

    // création threads producteurs
    int k = nb_producteurs - 1;
    while (k >= 0) {
        ids[k] = k;
        pthread_create(&producteurs[k], NULL, producteur, &ids[k]);
        k--;
    }
    // création threads consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, NULL);
    }

    // fin producteurs
    int j = 0;
    while (j < nb_producteurs) {
        pthread_join(producteurs[j], NULL);
        j++;
    }
    // fin consommateurs
    for (int i = nb_consommateurs - 1; i >= 0; i--) {
        pthread_join(consommateurs[i], NULL);
    }

    // "destruction" sémaphores + mutex
    sem_destroy(&sem_empty);
    sem_destroy(&sem_full);
    pthread_mutex_destroy(&mtx);

    return 0;
}
