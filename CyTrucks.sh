#!/bin/bash

# Chemins des dossiers
data_dir="data"
progc_dir="progc"
images_dir="images"
temp_dir="temp"
demo_dir="demo"

# Fonction pour vérifier et créer les dossiers manquants
check_and_create_directories() {
    local dirs=("$data_dir" "$progc_dir" "$images_dir" "$temp_dir" "$demo_dir")

    for dir in "${dirs[@]}"; do
        if [ ! -d "$dir" ]; then
            echo "Le dossier $dir est manquant, création en cours..."
            mkdir -p "$dir"
            echo "Dossier $dir créé avec succès."
        else
            echo "Le dossier $dir existe déjà."
        fi
    done

    # Vider le dossier temp s'il existe déjà
    if [ -d "$temp_dir" ]; then
        echo "Le dossier $temp_dir existe déjà, nettoyage en cours..."
        rm -rf "$temp_dir"/*
        echo "Nettoyage du dossier $temp_dir terminé."
    fi
}

# Fonction pour afficher l'aide
display_help() {
    echo "Usage: $0 chemin_du_fichier_CSV [-d1 | -d2 | -l | -t | -s]"
    echo "Options:"
    echo "  -d1 : Conducteurs avec le plus de trajets"
    echo "  -d2 : Conducteurs et la plus grande distance"
    echo "  -l  : Les 10 trajets les plus longs"
    echo "  -t  : Les 10 villes les plus traversées"
    echo "  -s  : Statistiques sur les étapes"
    echo "  -h  : Afficher cette aide"
    exit 1
}

# Fonction pour mesurer le temps d'exécution d'une commande
measure_execution_time() {
    local start_time=$(date +%s)
    "$@"
    local end_time=$(date +%s)
    local execution_time=$((end_time - start_time))
    echo "Temps d'exécution : $execution_time secondes"
}

# Vérification des paramètres d'entrée
if [ "$#" -eq 0 ]; then
    check_and_create_directories  # Créer les dossiers manquants si aucun argument n'est passé
    exit 0  # Quitte le script après avoir vérifié les dossiers
fi

# Récupération du chemin du fichier CSV (premier argument)
csv_file="$1"

# Vérification si l'option -h est passée, afficher l'aide
if [ "$csv_file" == "-h" ]; then
    display_help
fi

# Appel des fonctions pour vérifier et créer les dossiers
check_and_create_directories

# Timestamp de début
heure_debut=$(date +%s)

# Parcours des options passées en paramètres
while [ "$2" != "" ]; do
    case $2 in
        -d1 )
            # Traitement pour l'option -d1
            echo "Traitement D1..."
            
            # Filtrer les lignes correspondant au numéro de trajet "1" dans le fichier CSV
            grep ";1;" "$csv_file" > temp/temp.csv

            # Compter le nombre de trajets par conducteur
            awk -F';' '{count[$6]+=1} END {for (driver in count) print driver ";" count[driver]}' temp/temp.csv > temp/temp2.csv

            # Trier la liste par ordre décroissant du nombre de trajets
            sort -t';' -k2,2 -n -r temp/temp2.csv > temp/finaltemp.csv

            # Récupérer les 10 premiers conducteurs ayant effectué le plus grand nombre de trajets
            head -n 10 temp/finaltemp.csv > "$temp_dir/top_10_drivers.txt"

            # Créer le fichier de données pour l'histogramme et trier les données par nombre de trajets décroissant
            awk -v OFS=';' -F';' '{print $2, $1}' "$temp_dir/top_10_drivers.txt" | sort -t';' -k1,1nr | tac > "$temp_dir/histogram_data.csv"

            # Générer l'histogramme horizontal avec gnuplot
            gnuplot <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output '$images_dir/top_10_drivers.png'
set title "Top 10 Conducteurs avec le Plus de Trajets"
set xlabel "Nombre de Trajets"
set ylabel "Conducteurs"
set style fill solid
set yrange [-1:10]

# Utiliser les noms des conducteurs comme étiquettes y
set ytics nomirror
plot '$temp_dir/histogram_data.csv' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(stringcolumn(2)) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF

            # Afficher les 10 conducteurs avec le plus de trajets
            echo "Les 10 conducteurs avec le plus de trajets sont : "
            cat "$temp_dir/top_10_drivers.txt"

            # Nettoyer les fichiers temporaires
            rm temp/temp.csv temp/temp2.csv temp/finaltemp.csv "$temp_dir/histogram_data.csv"

            echo "Traitement D1 terminé."
            ;;
        -d2 )
            # Traitement pour l'option -d2
            echo "Traitement D2..."

            top_file="temp/top_10_driver_distances.csv"
            histogram_data="temp/histogram_data_distances.csv"
            output_image="images/top_10_driver_distances.png"

            # Calculer la distance totale parcourue par chaque conducteur et récupérer les 10 premiers
            awk -F';' 'NR>1 {total_distance[$6]+=$5} END {for (driver in total_distance) printf "%s;%f\n", driver, total_distance[driver]}' "$csv_file" \
            | sort -t';' -k2,2 -n -r | head -n 10 > "$top_file"

            # Créer le fichier de données pour l'histogramme
            awk -v OFS=';' -F';' '{print $2, $1}' "$top_file" | sort -t';' -k1,1nr | tac > "$histogram_data"

            # Générer l'histogramme horizontal avec gnuplot
            gnuplot <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output '$output_image'
set title "Top 10 Conducteurs avec la Plus Grande Distance Parcourue"
set xlabel "Distance (km)"
set ylabel "Conducteurs"
set style fill solid
set yrange [-1:10]

# Utiliser les noms des conducteurs comme étiquettes y
set ytics nomirror
plot '$histogram_data' using (\$1*0.5):0:(\$1*0.5):(0.4):yticlabels(stringcolumn(2)) with boxxyerrorbars lc rgbcolor 'spring-green' notitle
EOF

            # Afficher les 10 conducteurs avec la plus grande distance parcourue
            echo "Les 10 conducteurs avec la plus grande distance parcourue sont : "
            cat "$top_file"

            # Nettoyer les fichiers temporaires
            rm "$histogram_data" "$top_file"

            echo "Traitement D2 terminé."
            ;;
        -l )
            # Traitement pour l'option -l
            echo "Traitement -l..."

            top_distances_file="temp/top_10_distances.txt"

            # Utiliser awk pour calculer les distances et les stocker dans un fichier temporaire
            awk -F';' '{ distances[$1] += $5 } END { for (route_id in distances) print distances[route_id], route_id }' "$csv_file" | sort -k1,1nr | head -n 10 | sort -k2,2nr > "$top_distances_file"

            # Générer le graphique avec gnuplot
            gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/top_10_distances.png'
set title 'Top 10 Trajets avec les Plus Grandes Distances'
set ylabel 'Distance (km)'
set xlabel 'Trajets'
set style fill solid
set yrange [0:*]  # Ajustement de la plage y
set boxwidth 0.7

plot '$top_distances_file' using 1:xtic(2) with boxes lc rgbcolor 'spring-green' notitle
EOF

            # Afficher les 10 trajets avec les distances les plus grandes
            echo "Les 10 trajets avec les distances les plus grandes sont : "
            cat "$top_distances_file"

            # Nettoyer les fichiers temporaires
            rm "$top_distances_file"

            echo "Traitement -l terminé."
            ;;
        -t )
            # Traitement pour l'option -t
            echo "Traitement -t..."

            top_cities_file="temp/top_10_cities.txt"

            # Utiliser awk pour compter le nombre de trajets passant par chaque ville
            awk -F';' '{ cities[$3] += 1 } END { for (city in cities) print cities[city], city }' "$csv_file" | sort -k1,1nr | head -n 10 | sort -k2,2 > "$top_cities_file"

            # Générer le graphique avec gnuplot
            gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/top_10_cities.png'
set title 'Top 10 Villes les Plus Traversées'
set ylabel 'Nombre de Trajets'
set xlabel 'Villes'
set style fill solid
set yrange [0:*]  # Ajustement de la plage y
set boxwidth 0.7

plot '$top_cities_file' using 1:xtic(2) with boxes lc rgbcolor 'spring-green' notitle
EOF

            # Afficher les 10 villes les plus traversées
            echo "Les 10 villes les plus traversées sont : "
            cat "$top_cities_file"

            # Nettoyer les fichiers temporaires
            rm "$top_cities_file"

            echo "Traitement -t terminé."
            ;;
        -s )
            # Traitement pour l'option -s
            echo "Traitement -s..."

            steps_stats_file="temp/steps_stats.txt"

            # Utiliser awk pour calculer les statistiques sur le nombre d'étapes
            awk -F';' '{ steps[$4] += 1 } END { for (step in steps) print step, steps[step] }' "$csv_file" | sort -k2,2nr > "$steps_stats_file"

            # Générer le graphique avec gnuplot
            gnuplot -persist <<EOF
set terminal pngcairo enhanced font 'arial,10' size 700, 600
set output 'images/steps_stats.png'
set title 'Statistiques sur le Nombre d\'Étapes'
set ylabel 'Nombre d\'Étapes'
set xlabel 'Étapes'
set style fill solid
set yrange [0:*]  # Ajustement de la plage y
set boxwidth 0.7

plot '$steps_stats_file' using 2:xtic(1) with boxes lc rgbcolor 'spring-green' notitle
EOF

            # Afficher les statistiques sur le nombre d'étapes
            echo "Statistiques sur le nombre d'étapes : "
            cat "$steps_stats_file"

            # Nettoyer les fichiers temporaires
            rm "$steps_stats_file"

            echo "Traitement -s terminé."
            ;;
        * )
            echo "Option invalide : $2"
            display_help
            ;;
    esac
    shift
done

# Timestamp de fin
heure_fin=$(date +%s)

# Calcul de la durée d'exécution
duree=$((heure_fin - heure_debut))

# Affichage de la durée d'exécution
echo "Temps d'exécution : $duree secondes"
