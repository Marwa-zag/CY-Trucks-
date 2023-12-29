#!/bin/bash

# Chemin du fichier CSV (premier argument du script)
csv_file="data.csv"

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
head -n 10 temp/finaltemp.csv > temp/top_10_drivers.txt

# Générer le graphique avec gnuplot
gnuplot <<EOF
set term png
set output 'top_10_drivers.png'
set title "Top 10 Conducteurs avec le Plus de Trajets"
set xlabel "Nombre de Trajets"
set ylabel "Conducteurs"
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.5
set yrange [0.5:10.5]  # Ajustement de la plage y
set ytics out
plot 'temp/top_10_drivers.txt' using 2:xticlabels(1) with boxes title "Nombre de Trajets"
EOF

# Afficher les 10 conducteurs avec le plus de trajets
echo "Les 10 conducteurs avec le plus de trajets sont : "
cat temp/top_10_drivers.txt

# Nettoyer les fichiers temporaires
rm temp/temp.csv temp/temp2.csv

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
