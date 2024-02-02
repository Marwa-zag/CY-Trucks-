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

            # Assurez-vous que le fichier CSV est exécutable
            chmod 777 "$csv_file"

            # Timestamp de début
            start_time=$(date +%s)

            # Utilisation de awk pour créer un fichier temporaire avec les données nécessaires
            awk -F';' '
                NR > 1 {
                    if(!seen[$1,$3]++) town_a[$3]++;
                    if(!seen[$1,$4]++) town_b[$4]++;
                    if($2==1 && !depart_seen[$1,$3]++) depart[$3]++;
                }
                END {
                    for(town in town_a)
                        print town ";" town_a[town]+town_b[town] ";" (town in depart ? depart[town] : 0)
                }
            ' "$csv_file" > temp/tri.txt

            # Compilation et exécution du programme C
            gcc -o progc/t progc/t.c
            ./progc/t

            # Timestamp de fin
            end_time=$(date +%s)

            # Calcul du temps d'exécution en secondes
            elapsed_time=$((end_time - start_time))

            # Afficher le temps d'exécution
            echo "Temps en secondes : $elapsed_time."

            # Générer le graphique avec gnuplot
gnuplot -persist <<EOF
set terminal png size 1024,768
set output 'images/traitement_t.png'

set title "Option -t: Nb routes = f(Towns)"
set ylabel "NB ROUTES"
set xlabel "TOWN NAMES"

set style data histograms
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.75 relative

set xtics rotate by -45

# Define the data
set datafile separator ";"
plot 'temp/TriAlphabetique.txt' using 2:xtic(1) title 'Total routes' lt rgb "green", '' using 3 title 'First town' lt rgb "light-green"
EOF

            echo "Traitement -t terminé."
            ;;
        * )
            # Vérifier si l'option est -s
            if [ "$2" == "-s" ]; then
                # Exécution du script s à la place de l'option -s
                echo "Exécution du script s..."
                chmod 777 data/data.csv

                cut -d ';' -f1,2,5 data/data.csv > temp/s1.csv
                tail -n +2 temp/s1.csv > temp/s2.csv

                gcc -o progc/s progc/s.c
                ./progc/s temp/s2.csv

                head -n 50 temp/sortie.csv > temp/s.txt

                # Générer le graphique avec gnuplot
gnuplot -persist <<EOF
set terminal png size 1228,768
set output 'images/traitement_s.png'

set title "Option -s: Distance = f(Route)"
set ylabel "DISTANCE (km)"
set xlabel "ROUTE ID"

set style fill solid 0.5 border -1
set key left top

set xtics rotate by -45
set grid ytics

# Define the datafile separator
set datafile separator ";"

# Plot the data
plot 'temp/s.txt' using 2:xtic(1) with filledcurves x1 title 'Distances Max/Min (Km)', \
     '' using 3 with lines title 'Distance average (Km)'

EOF

                echo "Script s terminé."
            else
                echo "Option invalide : $2"
                display_help
            fi
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
