#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 chemin_du_fichier_CSV"
    exit 1
fi

# Chemin du fichier CSV (premier argument du script)
csv_file="$1"
top_distances_file="temp/top_10_distances.txt"

# Timestamp de début
heure_debut=$(date +%s)

echo "Traitement distances..."

# Utiliser awk pour calculer les distances et les stocker dans un fichier temporaire
awk -F';' '{ distances[$1] += $5 } END { for (route_id in distances) print distances[route_id], route_id }' "$csv_file" | sort -k1,1nr | head -n 10 | sort -k2,2nr > "$top_distances_file"

# Générer le graphique avec gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/traitement_l.png'
set title 'Option -l'
set ylabel 'DISTANCE (Km)'
set xlabel 'ROUTE ID'
set style fill solid
set yrange [0:*]  # Ajustement de la plage y
set boxwidth 0.7

plot '$top_distances_file' using 1:xtic(2) with boxes lc rgbcolor 'spring-green' notitle
EOF

# Afficher les 10 trajets avec les distances les plus grandes
echo "Les 10 trajets avec les distances les plus grandes sont : "
cat "$top_distances_file"

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
