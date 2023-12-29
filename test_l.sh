#!/bin/bash

# Vérifier si le fichier CSV est fourni en argument
if [ $# -eq 0 ]; then
    echo "Usage: $0 <fichier_CSV>"
    exit 1
fi

# Vérifier si le fichier existe
if [ ! -f "$1" ]; then
    echo "Le fichier $1 n'existe pas."
    exit 1
fi

# Extraire les données pertinentes du fichier CSV
awk -F',' 'NR > 1 { distance[$1] += $5 } END { for (id in distance) print id, distance[id] }' "$1" | sort -k2 -nr | head -n 10 > trajets_long.csv

# Afficher les 10 trajets les plus longs avec leurs distances
echo "Les 10 trajets les plus longs :"
cat trajets_long.csv

# Nettoyage du fichier temporaire
rm trajets_long.csv
