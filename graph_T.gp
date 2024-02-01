set terminal pngcairo enhanced font "arial,10" size 800,600
set output "graph_T.png"
set title 'Les 10 villes les plus traversées - Option -t'
set style data histogram
set style histogram clustered gap 1
set style fill solid border -1
set auto x
set yrange [0:*]
set xlabel 'Villes'
set ylabel 'Nombre de trajets'
set key top left

# Utilisation du format "#AARRGGBB" pour définir l'opacité des barres
set style fill transparent solid 0.5 noborder

# Faire pivoter les étiquettes de l'axe des x de 45 degrés
set xtics rotate by -45

plot 'resultat_T.txt' using 2:3:xtic(2) title 'Total Trajets' lc rgb '#0000FF', '' using 2:4 with lines lc rgb '#ADD8E6' title 'Départs'
