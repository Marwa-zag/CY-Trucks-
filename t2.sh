#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 chemin_du_fichier_CSV"
    exit 1
fi

# Chemin du fichier CSV (premier argument du script)
csv_file="$1"
top_file="temp/top_10_driver_distances.txt"

# Timestamp de début
heure_debut=$(date +%s)

echo "Traitement D2..."

# Calculer la distance totale parcourue par chaque conducteur
awk -F';' 'NR>1 {total_distance[$6]+=$5} END {for (driver in total_distance) printf "%s;%f\n", driver, total_distance[driver]}' "$csv_file" \
| sort -t';' -k2,2 -n -r > temp/driver_distances.txt

# Récupérer les 10 premiers conducteurs avec les plus grandes distances parcourues
head -n 10 temp/driver_distances.txt > "$top_file"

# Générer le graphique avec Gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 800, 600
set output 'images/top_10_driver_distances.png'
set title "Top 10 Conducteurs avec la Plus Grande Distance Parcourue"
set xlabel "Distance (km)"
set ylabel "Conducteurs"
set style fill solid 
set yrange [-1:10]
plot '$top_file' using (\$2):0:(\$2):(0.4):yticlabels(sprintf("%s", stringcolumn(2))) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF

# Afficher les 10 conducteurs avec les plus grandes distances parcourues
echo "Les 10 conducteurs avec les plus grandes distances parcourues sont : "
cat "$top_file"

# Nettoyer les fichiers temporaires
rm temp/driver_distances.txt

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
