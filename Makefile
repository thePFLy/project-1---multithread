# Partie 1 : Variables
DOSS_SRC_1 = part1
SOURCES = $(DOSS_SRC_1)/philosophes.c $(DOSS_SRC_1)/producteurs_consommateurs.c $(DOSS_SRC_1)/lecteurs_ecrivains.c
FICHIERS = $(DOSS_SRC_1)/philosophes $(DOSS_SRC_1)/producteurs_consommateurs $(DOSS_SRC_1)/lecteurs_ecrivains
DOSS_RESULTATS_1 = $(DOSS_SRC_1)/resultats
EVAL_PERF_1 = $(DOSS_SRC_1)/eval_perf.sh

# Partie 2 : Variables
DOSS_SRC_2 = part2
SOURCES = $(DOSS_SRC_2)/spinlock_test_tas.c $(DOSS_SRC_2)/spinlock_test_ttas.c $(DOSS_SRC_2)/semaphore_test.c
FICHIERS = $(DOSS_SRC_2)/spinlock_test_tas $(DOSS_SRC_2)/spinlock_test_ttas $(DOSS_SRC_2)/semaphore_test
DOSS_RESULTATS_2 = $(DOSS_SRC_2)/resultats
EVAL_PERF_2 = $(DOSS_SRC_2)/eval_perf.sh

# Cible principale
all: $(FICHIERS)

# Compilation partie 1
$(DOSS_SRC_1)/philosophes: $(DOSS_SRC_1)/philosophes.c
	gcc -Wall -pthread -o $@ $<
$(DOSS_SRC_1)/producteurs_consommateurs: $(DOSS_SRC_1)/producteurs_consommateurs.c
	gcc -Wall -pthread -o $@ $<
$(DOSS_SRC_1)/lecteurs_ecrivains: $(DOSS_SRC_1)/lecteurs_ecrivains.c
	gcc -Wall -pthread -o $@ $<

# Compilation partie 2
$(DOSS_SRC_2)/spinlock_test_tas: $(DOSS_SRC_2)/spinlock_test_tas.c
	gcc -Wall -pthread -o $@ $<
$(DOSS_SRC_2)/spinlock_test_ttas: $(DOSS_SRC_2)/spinlock_test_ttas.c
	gcc -Wall -pthread -o $@ $<
$(DOSS_SRC_2)/semaphore_test: $(DOSS_SRC_2)/semaphore_test.c
	gcc -Wall -pthread -o $@ $<

# Évaluation des performances
test: all
	/usr/bin/bash $(EVAL_PERF_1)
	/usr/bin/bash $(EVAL_PERF_2)

# Nettoyage
clean:
	rm -f $(FICHIERS)
	rm -rf $(DOSS_RESULTATS_1)
	rm -rf $(DOSS_RESULTATS_2)

# Aide
help:
	@echo "Commandes disponibles :"
	@echo "  make         : Compile tous les programmes."
	@echo "  make test    : Évaluation des performances."
	@echo "  make clean   : Nettoie les fichiers générés."
	@echo "  make help    : Affiche cette aide."
