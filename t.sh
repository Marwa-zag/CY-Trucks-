#!/bin/bash

# Assurez-vous que le fichier CSV est exécutable
chmod 777 data/data.csv

# Timestamp de début
start_time=$(date +%s)

# Utilisation de awk pour créer un fichier temporaire avec les données nécessaires
awk -F';' '
    NR > 1 {
        if(!seen[$1,$3]++) town_a[$3]++;
        if(!seen[$1,$4]++) town_b[$4]++;
        if($2==1 && !depart_seen[$1,$3]++) depart[$3]++;
    }
    END {
        for(town in town_a)
            print town ";" town_a[town]+town_b[town] ";" (town in depart ? depart[town] : 0)
    }
' data/data.csv > temp/tri.txt

# Compilation et exécution du programme C
gcc -o progc/t progc/t.c
./progc/t

# Timestamp de fin
end_time=$(date +%s)

# Calcul du temps d'exécution en secondes
elapsed_time=$((end_time - start_time))

# Afficher le temps d'exécution
echo "Temps en secondes : $elapsed_time s."

# Générer le graphique avec gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/villes_stats.png'
set title 'Statistiques sur les Villes'
set ylabel 'Nombre de Trajets'
set xlabel 'Villes'
set style fill solid
set yrange [0:*]  # Ajustement de la plage y
set boxwidth 0.7

plot 'temp/TriAlphabetique.txt' using 2:xtic(1) with boxes lc rgbcolor 'spring-green' title 'Nombre total de trajets', '' using 3:xtic(1) with boxes lc rgbcolor 'blue' title 'Nombre de départs'
EOF
