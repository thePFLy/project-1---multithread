#!/bin/bash
make clean
make
make test

cat part1/resultats/philosophes.csv
cat part1/resultats/lecteurs_ecrivains.csv
cat part1/resultats/producteurs_consommateurs.csv
cat part2/resultats/spinlock_test_tas.csv
cat part2/resultats/spinlock_test_ttas.csv
cat part2/resultats/semaphore_test.csv
cat part2/resultats/philosophes_attente_active.csv
cat part2/resultats/lecteurs_ecrivains_attente_active.csv
cat part2/resultats/producteurs_consommateurs_attente_active.csv