#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIGNES 10

// Structure pour stocker des données de trajet
typedef struct Data {
    int distance;// Distance parcourue
    char depart[50];// Lieu de départ
    char arrivee[50];// Lieu d'arrivée
    int id_etape;// Identifiant de l'étape
    int id_route;// Identifiant de la route
} Data;

Data trajet;
Data Dix_Lignes[MAX_LIGNES];

// Structure pour un nœud d'arbre AVL
typedef struct avl {
    int element;// Élément stocké (ici, id_etape)
    struct avl *fd;// Fils droit
    struct avl *fg;// Fils gauche
    int equilibre;// Facteur d'équilibre
    char depart[50];// Lieu de départ
    int id_route;// Identifiant de la route
} Avl;

// Crée et initialise un nouveau nœud AVL
Avl *creerAvl(int e, char dep[50], int id_un) {
    // Allocation de mémoire et vérification
    Avl *n = malloc(sizeof(Avl));
    if (n == NULL) {
        exit(1);// Arrêt du programme en cas d'échec de l'allocation
    }

    // Initialisation des valeurs du nœud
    n->element = e;
    n->fd = NULL;
    n->fg = NULL;
    n->equilibre = 0;
    strcpy(n->depart, dep);
    n->id_route = id_un;
    return n;
}

// Calcule le maximum de deux entiers
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Calcule le minimum de deux entiers
int min(int a, int b) {
    return (a < b) ? a : b;
}

// Calcule le maximum de trois entiers
int max2(int a, int b, int c) {
    return max(max(a, b), c);
}

// Calcule le minimum de trois entiers
int min2(int a, int b, int c) {
    return min(min(a, b), c);
}

// Effectue une rotation gauche sur un nœud AVL
Avl *rotationG(Avl *a) {
    Avl *pivot = a->fd;
    a->fd = pivot->fg;
    pivot->fg = a;
    int eq_a = a->equilibre;
    int eq_pivot = pivot->equilibre;
    a->equilibre = eq_a - max(eq_pivot, 0) - 1;
    pivot->equilibre = min2(eq_a - 2, eq_a + eq_pivot - 2, eq_pivot - 1);
    a = pivot;
    return a;
}

// Effectue une rotation droite sur un nœud AVL
Avl *rotationD(Avl *a) {
    Avl *pivot = a->fg;
    a->fg = pivot->fd;
    pivot->fd = a;
    int eq_a = a->equilibre;
    int eq_pivot = pivot->equilibre;
    a->equilibre = eq_a - min(eq_pivot, 0) + 1;
    pivot->equilibre = max2(eq_a + 2, eq_a + eq_pivot + 2, eq_pivot + 1);
    a = pivot;
    return a;
}


// Ajoute un fils droit à un nœud AVL
Avl *ajouterFD(Avl *a, int e, char dep[50], int id_un) {
    if (a == NULL) {
        return creerAvl(e, dep, id_un);
    }
    if (a->fd == NULL) {
        a->fd = creerAvl(e, dep, id_un);
    }
    return a;
}

// Ajoute un fils gauche à un nœud AVL
Avl *ajouterFG(Avl *a, int e, char dep[50], int id_un) {
    if (a == NULL) {
        return creerAvl(e, dep, id_un);
    }
    if (a->fg == NULL) {
        a->fg = creerAvl(e, dep, id_un);
    }
    return a;
}

// Effectue une double rotation gauche-droite
Avl *DoubleRG(Avl *a) {
    a->fd = rotationD(a->fd);
    return rotationG(a);
}

// Effectue une double rotation droite-gauche
Avl *DoubleRD(Avl *a) {
    a->fg = rotationG(a->fg);
    return rotationD(a);
}

// Équilibre un arbre AVL
Avl *equilibreAVL(Avl *a) {
    if (a->equilibre >= 2) {
        if (a->fd->equilibre >= 0) {
            return rotationG(a);
        } else {
            return DoubleRG(a);
        }
    } else if (a->equilibre <= -2) {
        if (a->fg->equilibre <= 0) {
            return rotationD(a);
        } else {
            return DoubleRD(a);
        }
    }
    return a;
}

// Parcours l'arbre AVL et stocke les données dans un tableau
void parcoursDecroissantAvecInfos(Avl *a, Data *donnees_triees, int *i, int limite) {
    if (a != NULL && *i < limite) {
        parcoursDecroissantAvecInfos(a->fd, donnees_triees, i, limite);

        if (*i < MAX_LIGNES) {
            donnees_triees[*i].id_etape = a->element;
            strcpy(donnees_triees[*i].depart, a->depart);
            donnees_triees[*i].id_route = a->id_route;
        } else {
            int minIndex = 0;
            for (int j = 1; j < MAX_LIGNES; ++j) {
                if (donnees_triees[j].id_etape < donnees_triees[minIndex].id_etape) {
                    minIndex = j;
                }
            }

            if (a->element > donnees_triees[minIndex].id_etape) {
                donnees_triees[minIndex].id_etape = a->element;
                strcpy(donnees_triees[minIndex].depart, a->depart);
                donnees_triees[minIndex].id_route = a->id_route;
            }
        }

        (*i)++;

        parcoursDecroissantAvecInfos(a->fg, donnees_triees, i, limite);
    }
}

// Insère un nouvel élément dans l'arbre AVL et l'équilibre
Avl *insertionAVL(Avl *a, int e, char dep[50], int id_un, int *h) {
    if (a == NULL) {
        *h = 1;
        return creerAvl(e, dep, id_un);
    } else if (e < a->element) {
        a->fg = insertionAVL(a->fg, e, dep, id_un, h);
        *h = -*h;
    } else if (e > a->element) {
        a->fd = insertionAVL(a->fd, e, dep, id_un, h);
    } else {
        *h = 0;
        return a;
    }
    if (*h != 0) {
        a->equilibre = a->equilibre + *h;
        a = equilibreAVL(a);
        if (a->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }
    return a;
}

// Effectue le tri des données en utilisant un arbre AVL
void Tri_t() {
    FILE *f, *Fich;
    f = fopen("temp/tri.txt", "r");
    Fich = fopen("temp/tri2.txt", "a");
    Avl *n = NULL;
    int hauteur = 0;

    do {
        fscanf(f, "%49[^;];%d;%d\n", trajet.depart, &trajet.id_etape, &trajet.id_route);
        n = insertionAVL(n, trajet.id_etape, trajet.depart, trajet.id_route, &hauteur);
    } while (!feof(f));

    int limite = 10;
    int i = 0;
    parcoursDecroissantAvecInfos(n, Dix_Lignes, &i, limite);
    for (int j = 0; j < MAX_LIGNES; ++j) {
        fprintf(Fich, "%s;%d;%d\n", Dix_Lignes[j].depart, Dix_Lignes[j].id_etape, Dix_Lignes[j].id_route);
    }

    fclose(f);
    fclose(Fich);
}

// Insère un nouvel élément dans l'arbre AVL en se basant sur l'ordre alphabétique
Avl *insertionAVL_Alphabetique(Avl *a, char dep[50], int id_etape, int id_un, int *h) {
    if (a == NULL) {
        *h = 1;
        return creerAvl(id_etape, dep, id_un);
    } else {
        int cmp = strcmp(dep, a->depart);
        if (cmp < 0) {
            a->fg = insertionAVL_Alphabetique(a->fg, dep, id_etape, id_un, h);
            *h = -*h;
        } else if (cmp > 0) {
            a->fd = insertionAVL_Alphabetique(a->fd, dep, id_etape, id_un, h);
        } else {
            *h = 0;
            return a;
        }
    }

    if (*h != 0) {
        a->equilibre = a->equilibre + *h;
        a = equilibreAVL(a);
        if (a->equilibre == 0) {
            *h = 0;
        } else {
            *h = 1;
        }
    }
    return a;
}

// Effectue le tri alphabétique des données en utilisant un arbre AVL
void Tri_tAlphabetique() {
    FILE *f, *Fich;
    f = fopen("temp/tri2.txt", "r");
    Fich = fopen("temp/TriAlphabetique.txt", "a");
    Avl *n = NULL;
    int hauteur = 0;

    do {
        fscanf(f, "%49[^;];%d;%d\n", trajet.depart, &trajet.id_etape, &trajet.id_route);
        n = insertionAVL_Alphabetique(n, trajet.depart, trajet.id_etape, trajet.id_route, &hauteur);
    } while (!feof(f));

    int limite = 10;
    int i = 0;
    parcoursDecroissantAvecInfos(n, Dix_Lignes, &i, limite);
    for (int j = MAX_LIGNES - 1; j >= 0; --j) {
        fprintf(Fich, "%s;%d;%d\n", Dix_Lignes[j].depart, Dix_Lignes[j].id_etape, Dix_Lignes[j].id_route);
    }

    fclose(f);
    fclose(Fich);
}

int main() {
    // Appel des fonctions de tri et affichage des résultats
    Tri_t();
    printf("Fichier temp/tri2.txt généré avec succès.\n");

    Tri_tAlphabetique();
    printf("Fichier temp/TriAlphabetique.txt généré avec succès.\n");

    // Affichage des données triées dans le terminal (par ordre croissant)
    printf("\nTop 10 des trajets triés par ordre alphabétique :\n");
    for (int i = MAX_LIGNES - 1; i >= 0; --i) {
        printf("%s;%d;%d\n", Dix_Lignes[i].depart, Dix_Lignes[i].id_etape, Dix_Lignes[i].id_route);
    }

    return 0;
}
