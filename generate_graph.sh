#!/bin/bash

# Compiler le programme C
make -f Makefile.txt

# Vérifier si la compilation a réussi
if [ $? -ne 0 ]; then
    echo "Erreur lors de la compilation du programme C."
    exit 1
fi

# Générer le fichier resultat_T.txt
./projet_info data.csv T

# Créer un script Gnuplot pour le graphique
cat > graph_T.gp <<EOF
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
EOF

# Lancer Gnuplot avec le script généré
gnuplot graph_T.gp

# Vérifier si la génération du graphique a réussi
if [ $? -ne 0 ]; then
    echo "Erreur lors de la génération du graphique avec Gnuplot."
    exit 1
fi

# Afficher un message
echo "Graphique généré avec succès. Image enregistrée sous le nom graph_T.png."
