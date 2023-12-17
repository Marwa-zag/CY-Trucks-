#!/bin/bash

# Chemins des dossiers
data_dir="data"
progc_dir="progc"
images_dir="images"
temp_dir="temp"
demo_dir="demo"

# Vérifier et créer les dossiers manquants
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

# Appel de la fonction pour vérifier et créer les dossiers manquants
check_and_create_directories

# Fonction pour compiler le programme C
compile_c_program() {
    cd "$progc_dir" || exit 1  # Se déplacer dans le répertoire du programme C

    if [ ! -f "mon_programme_c" ]; then
        echo "Le programme C n'est pas compilé. Compilation en cours..."
        make  # Utilisation de make pour la compilation (vous devez avoir un Makefile configuré)
        compile_status=$?  # Sauvegarder le statut de la compilation

        if [ $compile_status -eq 0 ] && [ -f "mon_programme_c" ]; then
            echo "Compilation terminée avec succès."
        else
            echo "Erreur lors de la compilation du programme C."
            exit 1
        fi
    else
        echo "Le programme C est déjà compilé."
    fi

    cd - > /dev/null  # Revenir au répertoire précédent
}

# Appel de la fonction pour vérifier et créer les dossiers manquants
check_and_create_directories

# Appel de la fonction pour compiler le programme C si nécessaire
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

# Vérifier si aucun argument n'est passé, afficher l'aide
if [ $# -eq 0 ]; then
    display_help
fi

# Récupérer le chemin du fichier CSV (premier argument)
csv_file=$1

# Vérifier si l'option -h est passée, afficher l'aide
if [ "$csv_file" == "-h" ]; then
    display_help
fi

# Vérifier si le fichier CSV existe
if [ ! -f "$csv_file" ]; then
    echo "Erreur : Le fichier CSV spécifié n'existe pas."
    exit 1
fi

# Parcourir les options passées en paramètres
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
