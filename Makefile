CFLAGS = -Wall -pthread
# partie
DOSS_SRC_1 = part1
SOURCES = $(DOSS_SRC_1)/philosophes.c $(DOSS_SRC_1)/producteurs_consommateurs.c $(DOSS_SRC_1)/lecteurs_ecrivains.c
FICHIERS = $(DOSS_SRC_1)/philosophes $(DOSS_SRC_1)/producteurs_consommateurs $(DOSS_SRC_1)/lecteurs_ecrivains
DOSS_RESULTATS_1 = part1/resultats
EVAL_PERF = $(DOSS_SRC_1)/eval_perf.sh


all: $(FICHIERS)
# Compilation partie 1
philosophes.out: $(DOSS_SRC_1)/philosophes.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/philosophes.c
producteurs_consommateurs.out: $(DOSS_SRC_1)/producteurs_consommateurs.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/producteurs_consommateurs.c
lecteurs_ecrivains.out: $(DOSS_SRC_1)/lecteurs_ecrivains.c
	gcc $(CFLAGS) -o $@ $(DOSS_SRC_1)/lecteurs_ecrivains.c

# Eval de performance
test: all
	/usr/bin/bash $(EVAL_PERF)   # Lancement du script de test dans le répertoire part1

# Effacer fichiers/dossiers
clean:
	rm -f $(FICHIERS)
	rm -rf $(DOSS_RESULTATS_1)

# help
help:
	@echo "cmds:"
	@echo "make: Compilation."
	@echo "make test: Evaluation de performance."
	@echo "make clean: Suppression fihiers/dossiers."
	@echo "make help: help."
