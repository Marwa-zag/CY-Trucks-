#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 chemin_du_fichier_CSV"
    exit 1
fi

# Chemin du fichier CSV (premier argument du script)
csv_file="$1"
top_file="temp/top_10_driver_distances.csv"
histogram_data="temp/histogram_data_distances.csv"
output_image="images/top_10_driver_distances.png"

# Timestamp de début
heure_debut=$(date +%s)

echo "Traitement D2..."

# Calculer la distance totale parcourue par chaque conducteur et récupérer les 10 premiers
awk -F';' 'NR>1 {total_distance[$6]+=$5} END {for (driver in total_distance) printf "%s;%f\n", driver, total_distance[driver]}' "$csv_file" \
| sort -t';' -k2,2 -n -r | head -n 10 > "$top_file"

# Créer le fichier de données pour l'histogramme
awk -v OFS=';' -F';' '{print $2, $1}' "$top_file" | sort -t';' -k1,1nr | tac > "$histogram_data"

# Générer l'histogramme horizontal avec gnuplot
gnuplot <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output '$output_image'
set title "Option -d2"
set xlabel "Distance (km)"
set ylabel "Conducteurs"
set style fill solid
set xrange [0:*]  # Remplacez * par une valeur appropriée selon vos données
set yrange [-1:10]
set ytics nomirror
plot '$histogram_data' using 1:0:(\$1):(0.4):yticlabels(stringcolumn(2)) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF

# Afficher les 10 conducteurs avec les plus grandes distances parcourues
echo "Les 10 conducteurs avec les plus grandes distances parcourues sont : "
cat "$top_file"

# Nettoyer les fichiers temporaires
rm "$histogram_data" "$top_file"

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"