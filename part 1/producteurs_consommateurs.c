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
int in = 0, out = 0;
sem_t empty, full;
pthread_mutex_t mutex;

void cpu_simulation() {
    for (int i = 0; i < 10000; i++);
}

// Pour chaque producteur
void *producteur(void *arg) {
    int id = *(int *)arg;
    for (int i = 0; i < PROD; i++) {
        sem_wait(&empty); 
        pthread_mutex_lock(&mutex);

        buffer[in] = id;
        in = (in + 1) % BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&full); 

        cpu_simulation();
    }
    return NULL;
}

void *consommateur(void *arg) {
    for (int i = 0; i < PROD; i++) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER;

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        // zone crit
        cpu_simulation();  
    }
    return NULL;
}

int main(int argc, char *argv[]) {
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
    sem_init(&empty, 0, BUFFER);  
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);
    // creation des thread producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        ids[i] = i;
        pthread_create(&producteurs[i], NULL, producteur, &ids[i]);
    }
    // creation thread consmmateur
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_create(&consommateurs[i], NULL, consommateur, NULL);
    }

    // fin producteurs
    for (int i = 0; i < nb_producteurs; i++) {
        pthread_join(producteurs[i], NULL);
    }
    // fin consommateurs
    for (int i = 0; i < nb_consommateurs; i++) {
        pthread_join(consommateurs[i], NULL);
    }
    // "destruction" semaphore + mutex
    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);

    return 0;
}
