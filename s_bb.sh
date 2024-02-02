#!/bin/bash

# Vérifier le nombre d'arguments
if [ "$#" -lt 1 ]; then
    echo "Usage: $0 <chemin_du_fichier_csv>"
    exit 1
fi

# Chemin du fichier CSV
csv_file="$1"

# Vérifier l'existence du fichier CSV
if [ ! -f "$csv_file" ]; then
    echo "Le fichier CSV n'existe pas."
    exit 1
fi

# Copier le fichier CSV dans le dossier 'data'
cp "$csv_file" data/

# Exécuter le programme C et générer le fichier de sortie CSV
gcc -o progc/programme_c progc/s_b.c -lm && progc/programme_c < "$csv_file" > temp/sortie.csv
exit_status=$?

# Vérifier si l'exécution du programme C a réussi
if [ $exit_status -ne 0 ]; then
    echo "L'exécution du programme C a échoué."
    exit 1
fi

# Exécuter le script AWK pour générer les graphiques
awk -F ';' 'NR > 1 {
    trajet = $1;
    distance = $5;
    if (trajet != "" && distance != "") {
        sum[trajet] += distance;
        if (distance < min[trajet] || min[trajet] == 0) {
            min[trajet] = distance;
        }
        if (distance > max[trajet]) {
            max[trajet] = distance;
        }
    }
}
END {
    for (trajet in sum) {
        moyenne = sum[trajet] / NR;
        printf "%d;%f;%f;%f;%f;%f\n", trajet, min[trajet], moyenne, max[trajet], sum[trajet], sum[trajet]/NR;
    }
}' temp/sortie.csv | sort -t';' -k4,4nr | head -n 50 > temp/templ.txt

# Utiliser Gnuplot pour générer le graphique
gnuplot <<EOF
set datafile separator ";"
set term png
set output "images/graphique_full.png"

# Définir les étiquettes et le titre du graphique
set xlabel "Identifiants des trajets"
set ylabel "Distances en km"
set title "Courbes min-max-moyenne des distances pour les 50 premiers trajets"

# Charger les données du fichier
plot "temp/templ.txt" using 1:2 with lines title "Distance minimale", \
     "temp/templ.txt" using 1:3 with lines title "Moyenne des distances", \
     "temp/templ.txt" using 1:4 with lines title "Distance maximale"
EOF

# Limiter le nombre de valeurs affichées à 50
mv images/graphique_full.png images/graphique.png

echo "Le script Shell a terminé avec succès."
