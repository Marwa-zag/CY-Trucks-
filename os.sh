#!/bin/bash

# Extraction des colonnes nécessaires du fichier CSV
cut -d':' -f1,2,5 data/data.csv > temp/s1.csv
tail -n +2 temp/s1.csv > temp/s2.csv

# Compilation du programme C
gcc -o progc/s progc/s.c

# Exécution du programme C avec le fichier CSV traité en argument
./progc/s temp/s2.csv

# Sélection des 50 premières lignes du fichier de sortie trié
head -n 50 temp/sortie.csv > temp/s.csv

# Utilisation de gnuplot pour générer le graphique
gnuplot <<EOF
set terminal png size 900, 600
set output 'images/data_s.png'
set datafile separator ";"
set title "Statistiques sur les étapes"
set border 4095 front lt black linewidth 1.000 dashtype solid
set xlabel 'Route ID'
set ylabel 'Distance (km)'
set xtics rotate by -80
Shadecolor = "light-green"
plot 'temp/s.csv' using 0:2:3:xticlabels(1) with filledcurves fill solid color rgb Shadecolor title "Distance Min/Max", \
     '' using 0:4:xticlabels(1) smooth mcspline lw 2 title "Distance average"
EOF

# Suppression des fichiers temporaires
rm temp/sortie.csv temp/s1.csv temp/s2.csv

# Affichage du graphique généré
display images/data_s.png

# Affichage d'un message
echo "Le graphique a été généré avec succès."
