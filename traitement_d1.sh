#!/bin/bash

# Vérification si l'option -d1 est utilisée
if [ "$1" != "-d1" ]; then
    echo "Usage: $0 -d1"
    exit 1
fi

# Extraction des données pour les conducteurs avec le nombre de trajets
awk -F ',' '{print $6}' data.csv | sort | uniq -c | sort -rn | head > top_10_conducteurs.txt

# Génération du graphique avec GNU Plot
echo "set terminal png" > conducteurs.gnu
echo "set output 'conducteurs.png'" >> conducteurs.gnu
echo "set title 'Top 10 des conducteurs avec le plus de trajets'" >> conducteurs.gnu
echo "set xlabel 'Nombre de trajets'" >> conducteurs.gnu
echo "set ylabel 'Conducteurs'" >> conducteurs.gnu
echo "set style data histogram" >> conducteurs.gnu
echo "set style histogram rowstacked" >> conducteurs.gnu
echo "set style fill solid border -1" >> conducteurs.gnu
echo "plot 'top_10_conducteurs.txt' using 2:xtic(1) with histogram title ''" >> conducteurs.gnu

# Exécution de GNU Plot
gnuplot conducteurs.gnu
