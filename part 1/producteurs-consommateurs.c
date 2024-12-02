/*
Tâche 1.2 – Coder le problème des producteurs-consommateurs :
- Le nombre de threads consommateur et le nombre de threads producteurs sont deux
paramètres obtenus à partir de la ligne de commande ;
- Le buffer est un tableau partagé de 8 places, contenant des entiers (int) :
o Une donnée produite ne doit jamais ‘écraser’ une donnée non consommée !
o À chaque insertion, chaque thread insère dans le buffer un int fixe, qui est son
identifiant.
- Entre chaque consommation ou production, un thread « simule » un traitement utilisant de
la ressource CPU, en utilisant : for (int i=0; i<10000; i++);
- Le traitement se fait en dehors de la zone critique.
- Le nombre d’éléments produits (et donc consommé) est toujours de 131072.
*/