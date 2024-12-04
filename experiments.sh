#!/bin/bash

# Appel à la commande make pour compiler les programmes
make

# Lancer les tests de performance (peut-être une cible spécifique du Makefile, comme 'make test')
make test

echo "=== Résultats Philosophes ==="
cat part1/resultats/philosophes.csv

echo "=== Résultats Lecteurs-Écrivains ==="
cat part1/resultats/lecteurs_ecrivains.csv

echo "=== Résultats Producteurs-Consommateurs ==="
cat part1/resultats/producteurs_consommateurs.csv
