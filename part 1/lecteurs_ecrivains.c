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