#!/bin/bash

# Chemin du fichier CSV (premier argument du script)
csv_file="data.csv"

# Timestamp de début
heure_debut=$(date +%s)

echo "Traitement D2..."

# Calculer la distance totale parcourue par chaque conducteur
awk -F';' 'NR>1 {total_distance[$6]+=$5} END {for (driver in total_distance) printf "%s;%f\n", driver, total_distance[driver]}' "$csv_file" \
| sort -t';' -k2,2 -n -r > temp/driver_distances.txt

# Récupérer les 10 premiers conducteurs avec les plus grandes distances parcourues
top_10_distances=$(head -n 10 temp/driver_distances.txt)

# Générer un fichier temporaire avec les 10 premiers conducteurs et leurs distances pour le graphique
echo "$top_10_distances" > temp/top_10_distances.txt

# Générer le graphique avec Gnuplot
gnuplot <<EOF
set term png
set output 'images/top_10_driver_distances.png'
set title "Top 10 Conducteurs avec la Plus Grande Distance Parcourue"
set xlabel "Distance parcourue"
set ylabel "Conducteurs"
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.5
set yrange [0.5:10.5]
set ytics out
plot 'temp/top_10_distances.txt' using 2:xticlabels(1) with boxes title "Distance parcourue"
EOF

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
