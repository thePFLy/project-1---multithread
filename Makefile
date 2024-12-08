CFLAGS = -Wall -pthread
# partie
DOSS_SRC_1 = part1
SOURCES = $(DOSS_SRC_1)/philosophes.c $(DOSS_SRC_1)/producteurs_consommateurs.c $(DOSS_SRC_1)/lecteurs_ecrivains.c
DOSS_RESULTATS_1 = part1/resultats
EVAL_PERF_1 = $(DOSS_SRC_1)/eval_perf.sh

# Partie 2 : Variables
DOSS_SRC_2 = part2
SOURCES = $(DOSS_SRC_2)/spinlock_test_tas.c $(DOSS_SRC_2)/spinlock_test_ttas.c $(DOSS_SRC_2)/semaphore_test.c $(DOSS_SRC_2)/philosophes_attente_active.c $(DOSS_SRC_2)/producteurs_consommateurs_attente_active.c $(DOSS_SRC_2)/lecteurs_ecrivains_attente_active.c
DOSS_RESULTATS_2 = $(DOSS_SRC_2)/resultats
EVAL_PERF_2 = $(DOSS_SRC_2)/eval_perf_2.sh

FICHIERS = $(DOSS_SRC_1)/philosophes $(DOSS_SRC_1)/producteurs_consommateurs $(DOSS_SRC_1)/lecteurs_ecrivains $(DOSS_SRC_2)/spinlock_test_tas $(DOSS_SRC_2)/spinlock_test_ttas $(DOSS_SRC_2)/semaphore_test $(DOSS_SRC_2)/philosophes_attente_active $(DOSS_SRC_2)/producteurs_consommateurs_attente_active $(DOSS_SRC_2)/lecteurs_ecrivains_attente_active

all: $(FICHIERS)
# Compilation partie 1
philosophes.out: $(DOSS_SRC_1)/philosophes.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/philosophes.c
producteurs_consommateurs.out: $(DOSS_SRC_1)/producteurs_consommateurs.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/producteurs_consommateurs.c
lecteurs_ecrivains.out: $(DOSS_SRC_1)/lecteurs_ecrivains.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/lecteurs_ecrivains.c

# Compilation partie 2
spinlock_test_tas.out: $(DOSS_SRC_2)/spinlock_test_tas.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/spinlock_test_tas.c
spinlock_test_ttas.out: $(DOSS_SRC_2)/spinlock_test_ttas.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/spinlock_test_ttas.c
semaphore_test.out: $(DOSS_SRC_2)/semaphore_test.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/semaphore_test.c
philosophes_attente_active.out: $(DOSS_SRC_2)/philosophes_attente_active.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/philosophes_attente_active.c
producteurs_consommateurs_attente_active.out: $(DOSS_SRC_2)/producteurs_consommateurs_attente_active.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/producteurs_consommateurs_attente_active.c
lecteurs_ecrivains_attente_active.out: $(DOSS_SRC_2)/lecteurs_ecrivains_attente_active.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_2)/lecteurs_ecrivains_attente_active.c

# Eval de performance
test: all
	/usr/bin/bash $(EVAL_PERF_1)   # Lancement du script de test dans le répertoire part1
	/usr/bin/bash $(EVAL_PERF_2)   # Lancement du script de test dans le répertoire part2

# Effacer fichiers/dossiers
clean:
	rm -f $(FICHIERS)
	rm -rf $(DOSS_RESULTATS_1)
	rm -rf $(DOSS_RESULTATS_2)

# help
help:
	@echo "cmds:"
	@echo "make: Compilation."
	@echo "make test: Evaluation de performance."
	@echo "make clean: Suppression fihiers/dossiers."
	@echo "make help: help."