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
}

# Fonction pour compiler le programme C
compile_c_program() {
    cd "$progc_dir" || exit 1

    if [ ! -f "mon_programme_c" ]; then
        echo "Le programme C n'est pas compilé. Compilation en cours..."
        make || { echo "Erreur lors de la compilation du programme C."; exit 1; }
        echo "Compilation terminée avec succès."
    else
        echo "Le programme C est déjà compilé."
    fi

    cd - > /dev/null
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

# Traitement pour l'option -d1
process_d1() {
    # Extraction des noms et nombres de trajets
    awk -F ';' '{print $8}' "$csv_file" | sort | uniq -c | sort -rn > "$temp_dir/temp_counted_data.txt"

    # Sélection des 10 premiers conducteurs
    head -n 10 "$temp_dir/temp_counted_data.txt" > "$temp_dir/top_10_drivers.txt"

    # Création du graphique au format PNG avec gnuplot
    gnuplot <<EOF
set term png
set output '$images_dir/top_10_drivers.png'
set title "Top 10 Conducteurs avec le Plus de Trajets"
set ylabel "Conducteurs"
set xlabel "Nombre de Trajets"
set style data histogram
set style histogram rowstacked
set style fill solid border -1
set boxwidth 0.5
plot '$temp_dir/top_10_drivers.txt' using 1:xticlabels(2) with boxes title "Nombre de Trajets"
EOF

    echo "Traitement D1 terminé."
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

# Appel des fonctions pour vérifier et créer les dossiers, et compiler le programme C
check_and_create_directories
compile_c_program

# Parcours des options passées en paramètres
while [ "$2" != "" ]; do
    case $2 in
        -d1 )
            # Appel de la fonction pour le traitement -d1
            process_d1
            ;;
        -d2 )
            # Traitement pour l'option -d2
            # ... (code pour le traitement -d2)
            ;;
        -l )
            # Traitement pour l'option -l
            # ... (code pour le traitement -l)
            ;;
        -t )
            # Traitement pour l'option -t
            # ... (code pour le traitement -t)
            ;;
        -s )
            # Traitement pour l'option -s
            # ... (code pour le traitement -s)
            ;;
        * )
            echo "Option invalide : $2"
            display_help
            ;;
    esac
    shift
done
