#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIGNES 10

typedef struct Data {
    int distance;
    char depart[50];
    char arrivee[50];
    int id_etape;
    int id_route;
} Data;

Data trajet;
Data Dix_Lignes[MAX_LIGNES];

typedef struct avl {
    int element;
    struct avl *fd;
    struct avl *fg;
    int equilibre;
    char depart[50];
    int id_route;
} Avl;

Avl *creerAvl(int e, char dep[50], int id_un) {
    Avl *n = malloc(sizeof(Avl));
    if (n == NULL) {
        exit(1);
    }
    n->element = e;
    n->fd = NULL;
    n->fg = NULL;
    n->equilibre = 0;
    strcpy(n->depart, dep);
    n->id_route = id_un;
    return n;
}

int max(int a, int b) {
    return (a > b) ? a : b;
}

int min(int a, int b) {
    return (a < b) ? a : b;
}

int max2(int a, int b, int c) {
    return max(max(a, b), c);
}

int min2(int a, int b, int c) {
    return min(min(a, b), c);
}

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

Avl *ajouterFD(Avl *a, int e, char dep[50], int id_un) {
    if (a == NULL) {
        return creerAvl(e, dep, id_un);
    }
    if (a->fd == NULL) {
        a->fd = creerAvl(e, dep, id_un);
    }
    return a;
}

Avl *ajouterFG(Avl *a, int e, char dep[50], int id_un) {
    if (a == NULL) {
        return creerAvl(e, dep, id_un);
    }
    if (a->fg == NULL) {
        a->fg = creerAvl(e, dep, id_un);
    }
    return a;
}

Avl *DoubleRG(Avl *a) {
    a->fd = rotationD(a->fd);
    return rotationG(a);
}

Avl *DoubleRD(Avl *a) {
    a->fg = rotationG(a->fg);
    return rotationD(a);
}

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
