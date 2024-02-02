#!/bin/bash

chmod 777 data/data.csv

cut -d ';' -f1,2,5 data/data.csv > temp/s1.csv
tail -n +2 temp/s1.csv > temp/s2.csv

gcc -o progc/s progc/s.c
./progc/s temp/s2.csv

head -n 50 temp/sortie.csv > temp/s.txt

# Générer le graphique avec gnuplot
gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/steps_stats.png'
set title 'Statistiques sur le Nombre d''Étapes'
set ylabel 'Nombre d''Étapes'
set xlabel 'Étapes'
set style fill solid
set boxwidth 0.7

plot 'temp/s.txt' using 2:xtic(1) with boxes lc rgbcolor 'spring-green' notitle
EOF
