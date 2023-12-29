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

# Appel des fonctions pour vérifier et créer les dossiers, et compiler le programme C
check_and_create_directories
compile_c_program

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

# Vérification des paramètres d'entrée
if [ $# -eq 0 ]; then
    display_help
fi

# Récupération du chemin du fichier CSV (premier argument)
csv_file=$1

# Vérification si l'option -h est passée, afficher l'aide
if [ "$csv_file" == "-h" ]; then
    display_help
fi

# Vérification si le fichier CSV existe
if [ ! -f "$csv_file" ]; then
    echo "Erreur : Le fichier CSV spécifié n'existe pas."
    exit 1
fi

# Parcours des options passées en paramètres
while [ "$2" != "" ]; do
    case $2 in
        -d1 | -d2 | -l | -t | -s )
            # Traitements en fonction des options passées
            option=$2
            echo "Traitement pour l'option $option à ajouter ici..."
            ;;
        * )
            echo "Option invalide : $2"
            display_help
            ;;
    esac
    shift
done
