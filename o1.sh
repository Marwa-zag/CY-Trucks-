#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 chemin_du_fichier_CSV"
    exit 1
fi

# Chemin du fichier CSV (premier argument du script)
csv_file="$1"
top_file="temp/top_10_drivers.csv"
histogram_data="temp/histogram_data.csv"
output_image="images/top_10_drivers.png"

# Timestamp de début
heure_debut=$(date +%s)

echo "Traitement D1..."

# Filtrer les lignes correspondant au numéro de trajet "1" dans le fichier CSV
grep ";1;" "$csv_file" > temp/temp.csv

# Compter le nombre de trajets par conducteur
awk -F';' '{count[$6]+=1} END {for (driver in count) print driver ";" count[driver]}' temp/temp.csv > temp/temp2.csv

# Trier la liste par ordre décroissant du nombre de trajets
sort -t';' -k2,2 -n -r temp/temp2.csv > temp/finaltemp.csv

# Récupérer les 10 premiers conducteurs ayant effectué le plus grand nombre de trajets
head -n 10 temp/finaltemp.csv > "$top_file"

# Créer le fichier de données pour l'histogramme et trier les données par nombre de trajets décroissant
awk -v OFS=';' -F';' '{print $2, $1}' "$top_file" | sort -t';' -k1,1nr | tac > "$histogram_data"

# Générer l'histogramme horizontal avec gnuplot
gnuplot <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output '$output_image'
set title "Top 10 Conducteurs avec le Plus de Trajets"
set xlabel "Nombre de Trajets"
set ylabel "Drivers"
set style fill solid
set yrange [-1:10]

# Utiliser les noms des conducteurs comme étiquettes y
set ytics nomirror
plot '$histogram_data' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(stringcolumn(2)) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF

# Afficher les 10 conducteurs avec le plus de trajets
echo "Les 10 conducteurs avec le plus de trajets sont : "
cat "$top_file"

# Nettoyer les fichiers temporaires
rm temp/temp.csv temp/temp2.csv temp/finaltemp.csv "$histogram_data"

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
