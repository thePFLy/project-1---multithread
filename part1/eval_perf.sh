#!/bin/bash
mkdir -p part1/resultats

test() {
    local program=$1
    local threads=$2
    local output_file=$3

    echo "Threads,Temps moyen (s)" > "$output_file"

    for t in $threads; do
        times=()
        # Cas où deux types de threads (diviser par 2)
        if [[ "$program" == "part1/producteurs_consommateurs" || "$program" == "part1/lecteurs_ecrivains" ]]; then
            half_t=$((t / 2))  # diviser le nombre total de threads en deux
            for i in {1..5}; do
                t_passed=$(/usr/bin/time -f "%e" ./$program $half_t $half_t 2>&1)
                [[ "$t_passed" =~ ^[0-9]+(\.[0-9]+)?$ ]] && times+=("$t_passed") || echo "Erreur : temps invalide pour $program"
            done
        else
            for i in {1..5}; do
                t_passed=$(/usr/bin/time -f "%e" ./$program $t 2>&1)
                [[ "$t_passed" =~ ^[0-9]+(\.[0-9]+)?$ ]] && times+=("$t_passed") || echo "Erreur : temps invalide pour $program"
            done
        fi

        # Calculer du temps moyen
        avg_time=$(echo "${times[@]}" | awk '{s=0; for(i=1;i<=NF;i++) s+=$i; print s/NF}')
        formatted_time=$(LC_NUMERIC=C printf "%.3f" "$avg_time")
        echo "$t,$formatted_time" >> "$output_file"
    done
}

test "part1/philosophes" "2 4 8 16 32" "part1/resultats/philosophes.csv"
test "part1/lecteurs_ecrivains" "2 4 8 16 32" "part1/resultats/lecteurs_ecrivains.csv"
test "part1/producteurs_consommateurs" "2 4 8 16 32" "part1/resultats/producteurs_consommateurs.csv"
