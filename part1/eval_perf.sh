# Tâche 1.4 - Écrire un script d’évaluation des performances
# - Sur le modèle du précédent TD, mesurer la performance de chacun des trois programmes et
# la sauver dans des fichiers .csv en faisant attention à :
# o Désactiver toute sortie sur STDOUT (car cela diminuerait les performances) ;
# o Prendre 5 mesures pour chaque configuration avec les nombres de threads TOTAUX
# suivants: [2, 4, 8, 16, 32]
# Pour les problèmes avec deux types distincts de threads, vous devez donc séparer ce nombre en 2 ensembles de threads de taille égale.
#

#!/bin/bash
exec 1>/dev/null
mkdir -p part1/resultats

test() {
    local program=$1
    local threads=$2
    local output_file=$3

    echo "Threads,Temps moyen (s)" > "$output_file"

    for t in $threads; do
        total_time=0

        # 2 types de thread (diviser par 2)
        if [[ "$program" == "part1/producteurs_consommateurs" || "$program" == "part1/lecteurs_ecrivains" ]]; then
            half_t=$((t / 2))
            for i in {1..5}; do
                elapsed=$(/usr/bin/time -f "%e" ./$program $half_t $half_t 2>&1)
                total_time=$(echo "$total_time + $elapsed" | bc -l)
            done
        else
            for i in {1..5}; do
                elapsed=$(/usr/bin/time -f "%e" ./$program $t 2>&1)
                total_time=$(echo "$total_time + $elapsed" | bc -l)
            done
        fi

        # moyenne
        avg_time=$(echo "$total_time / 5" | bc -l)
        formatted_time=$(LC_NUMERIC=C printf "%.3f" "$avg_time")
        
        echo "$t,$formatted_time" >> "$output_file"
    done
}

test "part1/philosophes" "2 4 8 16 32" "part1/resultats/philosophes.csv"
test "part1/lecteurs_ecrivains" "2 4 8 16 32" "part1/resultats/lecteurs_ecrivains.csv"
test "part1/producteurs_consommateurs" "2 4 8 16 32" "part1/resultats/producteurs_consommateurs.csv"
