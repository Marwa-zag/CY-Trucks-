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
longest_10_drivers=$(head -n 10 temp/finaltemp.csv)
echo "Les 10 conducteurs avec le plus de trajets sont : "
echo "$longest_10_drivers"

# Nettoyer les fichiers temporaires
rm temp/temp.csv temp/finaltemp.csv temp/temp2.csv

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
