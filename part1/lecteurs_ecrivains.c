/*
Tâche 1.3 – Coder le problème des lecteurs et écrivains (code du TD) :
- Le nombre de threads écrivains et le nombre de threads lecteurs sont deux paramètres
obtenus à partir de la ligne de commande ;
- Un écrivain ou un lecteur « simule » un accès en écriture ou en lecture à la base de données
avec l’opération : for (int i=0; i<10000; i++); il n’y a pas d’attente entre deux
tentatives d’accès.
- Le(s) écrivain(s) effectue(nt) 640 écritures et le(s) lecteur(s)effectue(nt) 2540 lectures au
total.
- Contrairement au problème des producteurs-consommateurs, le traitement se fait DANS
la zone critique.
*/

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

#define TOTAL_WRITES 640
#define TOTAL_READS 2540

pthread_mutex_t mutex_reader = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_writer = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t z = PTHREAD_MUTEX_INITIALIZER;

sem_t db_reader;
sem_t db_writer;

int writercount = 0;
int readcount = 0;

int writes_done = 0;
int reads_done = 0;

void process() {
    for (int i = 0; i < 10000; i++);
}

// Fonction pour les lecteurs
void* reader(void* arg) {
    while (1) {
        pthread_mutex_lock(&z); 
        sem_wait(&db_reader);
        pthread_mutex_lock(&mutex_reader);
        
        if (reads_done >= TOTAL_READS) {
            pthread_mutex_unlock(&mutex_reader);
            sem_post(&db_reader);
            pthread_mutex_unlock(&z);
            break;
        }
        
        readcount++;
        if (readcount == 1) {
            sem_wait(&db_writer);
        }
        
        pthread_mutex_unlock(&mutex_reader);
        sem_post(&db_reader);
        pthread_mutex_unlock(&z);

        // Section critique : lecture
        process();
        pthread_mutex_lock(&mutex_reader);
        reads_done++;
        pthread_mutex_unlock(&mutex_reader);

        pthread_mutex_lock(&mutex_reader);
        readcount--;
        if (readcount == 0) {
            sem_post(&db_writer);
        }
        pthread_mutex_unlock(&mutex_reader);
    }
    return NULL;
}

// Fonction pour les écrivains
void* writer(void* arg) {
    while (1) {
        pthread_mutex_lock(&mutex_writer);
        
        if (writes_done >= TOTAL_WRITES) {
            pthread_mutex_unlock(&mutex_writer);
            break;
        }
        
        writercount++;
        if (writercount == 1) {
            sem_wait(&db_reader);
        }
        
        pthread_mutex_unlock(&mutex_writer);
        sem_wait(&db_writer);

        // Section critique : écriture
        process();
        pthread_mutex_lock(&mutex_writer);
        writes_done++;
        pthread_mutex_unlock(&mutex_writer);

        sem_post(&db_writer);
        pthread_mutex_lock(&mutex_writer);
        writercount--;
        if (writercount == 0) {
            sem_post(&db_reader);
        }
        pthread_mutex_unlock(&mutex_writer);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    // arguments (erreur)
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <nb_lecteurs> <nb_ecrivains>\n", argv[0]);
        return EXIT_FAILURE;
    }

    int nb_readers = atoi(argv[1]);
    int nb_writers = atoi(argv[2]);

    if (nb_readers <= 0 || nb_writers <= 0) {
        fprintf(stderr, "Le nombre de lecteurs et d'écrivains doit être supérieur à 0.\n");
        return EXIT_FAILURE;
    }

    pthread_t readers[nb_readers];
    pthread_t writers[nb_writers];

    sem_init(&db_reader, 0, 1);
    sem_init(&db_writer, 0, 1);

    for (int i = 0; i < nb_readers; i++) {
        pthread_create(&readers[i], NULL, reader, NULL);
    }

    for (int i = 0; i < nb_writers; i++) {
        pthread_create(&writers[i], NULL, writer, NULL);
    }

    for (int i = 0; i < nb_readers; i++) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < nb_writers; i++) {
        pthread_join(writers[i], NULL);
    }

    sem_destroy(&db_reader);
    sem_destroy(&db_writer);
    pthread_mutex_destroy(&mutex_reader);
    pthread_mutex_destroy(&mutex_writer);
    pthread_mutex_destroy(&z);

    return EXIT_SUCCESS;
}