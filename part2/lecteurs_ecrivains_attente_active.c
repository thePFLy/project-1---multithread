#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdatomic.h>

#define TOTAL_WRITES 640
#define TOTAL_READS 2540

pthread_spinlock_t spinlock_reader;
pthread_spinlock_t spinlock_writer;

atomic_int writercount = 0;
atomic_int readcount = 0;

atomic_int writes_done = 0;
atomic_int reads_done = 0;

void process() {
    for (int i = 0; i < 10000; i++);
}

// Fonction pour les lecteurs
void* reader(void* arg) {
    while (1) {
        pthread_spin_lock(&spinlock_reader);
        
        if (atomic_load(&reads_done) >= TOTAL_READS) {
            pthread_spin_unlock(&spinlock_reader);
            break;
        }

        atomic_fetch_add(&readcount, 1);
        if (atomic_load(&readcount) == 1) {
            while (atomic_load(&writercount) > 0);
        }

        pthread_spin_unlock(&spinlock_reader);

        // Section critique : lecture
        process();

        pthread_spin_lock(&spinlock_reader);
        atomic_fetch_add(&reads_done, 1);
        atomic_fetch_sub(&readcount, 1);
        pthread_spin_unlock(&spinlock_reader);
    }
    return NULL;
}

// Fonction pour les écrivains
void* writer(void* arg) {
    while (1) {
        pthread_spin_lock(&spinlock_writer);
        
        if (atomic_load(&writes_done) >= TOTAL_WRITES) {
            pthread_spin_unlock(&spinlock_writer);
            break;
        }

        atomic_fetch_add(&writercount, 1);
        pthread_spin_unlock(&spinlock_writer);

        while (atomic_load(&readcount) > 0);

        // Section critique : écriture
        process();

        pthread_spin_lock(&spinlock_writer);
        atomic_fetch_add(&writes_done, 1);
        atomic_fetch_sub(&writercount, 1);
        pthread_spin_unlock(&spinlock_writer);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
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

    pthread_spin_init(&spinlock_reader, 0);
    pthread_spin_init(&spinlock_writer, 0);

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

    pthread_spin_destroy(&spinlock_reader);
    pthread_spin_destroy(&spinlock_writer);

    return EXIT_SUCCESS;
}