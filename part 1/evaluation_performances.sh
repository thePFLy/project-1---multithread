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
mkdir -p resultats

test() {
    local program=$1
    local threads=$2
    local output_file=$3

    # csv
    echo "Threads,Temps moyen (s)" > "$output_file"

    for t in $threads; do
        total_time=0

        # Cas ou deux types de threads
        if [[ "$program" == "producteurs_consommateurs" || "$program" == "lecteurs_ecrivains" ]]; then
            half_t=$((t / 2))  # consignes: Diviser le nombre total de threads en deux groupes égaux
            for i in {1..5}; do
                start=$(LC_NUMERIC=C date +%s.%N)
                ./$program $half_t $half_t
                end=$(LC_NUMERIC=C date +%s.%N)
                elapsed=$(echo "$end - $start" | bc -l)  # temps écoulé
                total_time=$(echo "$total_time + $elapsed" | bc -l)
            done
        else
            for i in {1..5}; do
                start=$(LC_NUMERIC=C date +%s.%N)
                ./$program $t
                end=$(LC_NUMERIC=C date +%s.%N)
                elapsed=$(echo "$end - $start" | bc -l)  # temps écoulé
                total_time=$(echo "$total_time + $elapsed" | bc -l)
            done
        fi

        # Calculer du temps, 3 décimales
        avg_time=$(echo "$total_time / 5" | bc -l)
        formatted_time=$(LC_NUMERIC=C printf "%.3f" "$avg_time")

        # Ajouter au csv
        echo "$t,$formatted_time" >> "$output_file"
    done
}

test "philosophes" "2 4 8 16 32" "resultats/philosophes.csv"
test "lecteurs_ecrivains" "2 4 8 16 32" "resultats/lecteurs_ecrivains.csv"
test "producteurs_consommateurs" "2 4 8 16 32" "resultats/producteurs_consommateurs.csv"