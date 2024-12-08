#!/bin/bash
mkdir -p part2/resultats

test() {
    local program=$1
    local threads=$2
    local output_file=$3

    # csv
    echo "Threads,Temps moyen (s)" > "$output_file"

    for t in $threads; do
        total_time=0

        # Cas où deux types de threads (diviser par 2)
        if [[ "$program" == "part2/producteurs_consommateurs_attente_active" || "$program" == "part2/lecteurs_ecrivains_attente_active" ]]; then
            half_t=$((t / 2))  # diviser le nombre total de threads en deux
            for i in {1..5}; do
                # Mesurer le temps avec /usr/bin/time
                t_passed=$(/usr/bin/time -f "%e" ./$program $half_t $half_t 2>&1)
                total_time=$(echo "$total_time + $t_passed" | bc -l)
            done
        else
            for i in {1..5}; do
                # Mesurer le temps avec /usr/bin/time
                t_passed=$(/usr/bin/time -f "%e" ./$program $t 2>&1)
                total_time=$(echo "$total_time + $t_passed" | bc -l)
            done
        fi

        # Calculer du temps moyen, 3 décimales
        avg_time=$(echo "$total_time / 5" | bc -l)
        formatted_time=$(LC_NUMERIC=C printf "%.3f" "$avg_time")

        # Ajouter au fichier CSV
        echo "$t,$formatted_time" >> "$output_file"
    done
}

# Lancer les tests pour chaque programme
test "part2/spinlock_test_tas" "1 2 4 8 16 32" "part2/resultats/spinlock_test_tas.csv"
test "part2/spinlock_test_ttas" "1 2 4 8 16 32" "part2/resultats/spinlock_test_ttas.csv"
test "part2/philosophes_attente_active" "2 4 8 16 32" "part2/resultats/philosophes_attente_active.csv"
test "part2/producteurs_consommateurs_attente_active" "2 4 8 16 32" "part2/resultats/producteurs_consommateurs_attente_active.csv"
test "part2/lecteurs_ecrivains_attente_active" "2 4 8 16 32" "part2/resultats/lecteurs_ecrivains_attente_active.csv"
