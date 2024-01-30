#include "AVL_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//------------------------------ Traitement t ---------------------//

#define ligne_taille_max 5000
char chemin_fichierT[50] = "./data/";

void afficherArbre(Arbre *node) 
{
    if (node != NULL) {
        afficherArbre(node->left);
        printf("%s ", node->nomVille);
        afficherArbre(node->right);
    }
}


void traitement_t(char *fichier) 
{
    if (strcmp(chemin_fichierT, "./data/") == 0) {
        strcat(chemin_fichierT, fichier);
        strcat(chemin_fichierT, ".csv");
    }

    FILE* file = fopen(chemin_fichierT, "r");
    if (file == NULL) {
        perror("ERREUR : impossible d'ouvrir le fichier csv");
        exit(EXIT_FAILURE);
    }

    char ligne[ligne_taille_max];

    // Ignorer la première ligne -> présentation des colonnes
    fgets(ligne, ligne_taille_max, file);

    char TownA[50];
    char TownB[50];

    int* h = malloc(sizeof(int));
    *h = 0;
    Arbre* node = NULL;

    while (fgets(ligne, ligne_taille_max, file) != NULL) {
        char *token = strtok(ligne, ";");
        if (token != NULL) {
            // Skip les deux premiers champs 
            token = strtok(NULL, ";");
            token = strtok(NULL, ";");

            // Les prochains deux champs contiennent les noms de ville
            if (token != NULL) {
                // Vérif que la chaîne est correctement copiée et finie par le caractère nul
                strncpy(TownA, token, sizeof(TownA) - 1);
                TownA[sizeof(TownA) - 1] = '\0';
            }

            token = strtok(NULL, ";");
            if (token != NULL) {
                // Vérif que la chaîne est correctement copiée et finie par le caractère nul
                strncpy(TownB, token, sizeof(TownB) - 1);
                TownB[sizeof(TownB) - 1] = '\0';

                // Affichage pour le débogage
                printf("Insertion de %s et %s\n", TownA, TownB);

                node = insertion(node, TownA, h, 0); 
                node = equilibrageAVL(node);

                node = insertion(node, TownB, h, 1); 
                node = equilibrageAVL(node);

                // Affichage pour le débogage
                printf("Arbre après insertion :\n");
                afficherArbre(node);
                printf("\n");
            }
        }
    }

    fclose(file);

    // Avoir les 10 villes les plus visitées
    Ville tableau[10];

    int* z = malloc(sizeof(int));
    *z = 0;
    postfixeFilsDroit(node, tableau, z); 

    freeTree(node);
    free(h);
    free(z);

    // On met les infos dans le fichier .txt temporaire
    FILE* fichier_temp;
    fichier_temp = fopen("./temp/data_t.txt", "w");
    if (fichier_temp == NULL) {
        perror("ERREUR : impossible d'ouvrir le fichier temporaire");
        exit(EXIT_FAILURE);
    }

    // On met les infos dans le fichier .txt temporaire
    for (int y = 0; y < 10; y++) {
        fprintf(fichier_temp, "%s;%d;%d\n", tableau[y].nomVille, tableau[y].nbTrajets, tableau[y].nbDepart);
    }
    fclose(fichier_temp);
}


//----------------------------------------------------------------//





//
// Creer arbre
Arbre* creerArbre(char* nomVille) {
  Arbre *node = malloc(sizeof(Arbre));
  if (node == NULL) 
  {
    fprintf(stderr, "Erreur d'allocation mémoire\n");
    exit(EXIT_FAILURE);
  }

  strcpy(node->nomVille, nomVille);
  node->left = NULL;
  node->right = NULL;
  node->nbrTrajets = 0;
  node->nbrDepart = 0;
  node->eq = 0;
  return node;
}


//                              Operations

// Obtenir les 10 plus grandes villes:
void postfixeFilsDroit(Arbre* node, Ville tableau[], int *i) 
{
  if (node == NULL || *i >= 10) 
  {
    return;
  }

  // Parcourir le fils droit en premier
  postfixeFilsDroit(node->right, tableau, i);

  // Vérifier à nouveau la condition après le parcours du fils droit
  if (*i >= 10) 
  {
    return;
  }

  // Parcourir le fils gauche après avoir traité le fils droit
  postfixeFilsDroit(node->left, tableau, i);

  tableau[*i].nbDepart = node->nbrDepart;
  tableau[*i].nbTrajets = node->nbrTrajets;
  strcpy(tableau[*i].nomVille, node->nomVille);
  (*i)++;
}

// Insertion
Arbre* insertion(Arbre* node, char* nomVille, int* h, int depart) 
{
    if (node == NULL)
    {
        *h = 1;
        node = creerArbre(nomVille);
        node->nbrTrajets = 1;
        if (depart == 1) {
            node->nbrDepart = 1;
        } else {
            node->nbrDepart = 0;
        }
        return node;
    }

    int comparaison = strcmp(nomVille, node->nomVille);

    if (comparaison != 0)
    {
        if (comparaison < 0) {
            node->left = insertion(node->left, nomVille, h, depart);
            *h = -*h;
        } else {
            node->right = insertion(node->right, nomVille, h, depart);
        }
    }
    else
    {
        *h = 0;
        // La ville existe déjà -> +1 nbr de fois où elle est traversée
        node->nbrTrajets++;

        if (depart == 1) {
            // Si la ville est une ville de départ, incrémenter le compteur de départs
            node->nbrDepart++;
        }
    }

    // Mettre à jour l'équilibre du nœud après l'insertion
    int eq_left = (node->left != NULL) ? node->left->eq : 0;
    int eq_right = (node->right != NULL) ? node->right->eq : 0;
    node->eq = eq_left - eq_right;

    if (*h != 0)
    {
        if (node->eq == 0)
        {
            *h = 0;
        }
        else
        {
            *h = 1;
        }
    }

    return node;
}



//-----------------------------------Equilibrage----------------------------------------

// Cas 1 : ele. aj. tt a droite
Arbre* rotationGauche(Arbre* a)
{
    if (a == NULL || a->right == NULL || a->right->left == NULL) {
        fprintf(stderr, "Erreur : rotationGauche appelée avec un nœud ou fils droit ou fils gauche NULL\n");
        return a;
    }

    Arbre* pivot = a->right;
    Arbre* filsGauchePivot = pivot->left;

    if (filsGauchePivot == NULL) {
        fprintf(stderr, "Erreur : le fils gauche du pivot est NULL\n");
        return a;
    }

    printf("Avant rotation gauche - Nœud a : %s, Fils droit : %s\n", a->nomVille, pivot->nomVille);

    a->right = filsGauchePivot;
    pivot->left = a;

    // Mettre à jour les champs eq après la rotation
    int eq_a = a->eq;
    int eq_p = pivot->eq;
    a->eq = eq_a - fmax(eq_p, 0) - 1;
    pivot->eq = fmin(fmin(eq_a - 2, eq_a + eq_p - 2), eq_p - 1);

    printf("Après rotation gauche - Nœud a : %s, Fils droit : %s\n", pivot->nomVille, filsGauchePivot->nomVille);

    return pivot;  // Retourner le nouveau nœud pivot
}


//Cas 2: ele. aj. tt a gauche
Arbre* rotationDroite(Arbre* a)
{
  Arbre* pivot;
  int eq_a, eq_p;

  pivot = a->left;
  a->left = pivot->right;
  pivot->right = a;
  eq_a = a->eq;
  eq_p = pivot->eq;
  a->eq = eq_a - fmin(eq_p, 0) + 1;
  pivot->eq = fmax(fmax(eq_a + 2 , eq_a+eq_p+2), eq_p + 1);
  a = pivot;
  return a;
}

//Rota double
Arbre* rotationDoubleGauche(Arbre* a)
{
  a->right = rotationDroite(a->right);
  return rotationGauche(a);
}

Arbre* rotationDoubleDroit(Arbre* a)
{
  a->left = rotationGauche(a->left);
  return rotationDroite(a);
}

// Equilibrage
Arbre* equilibrageAVL(Arbre* a)
{
    if (a != NULL)
    {
        // Équilibrer les sous-arbres
        a->left = equilibrageAVL(a->left);
        a->right = equilibrageAVL(a->right);

        int eq_a = a->eq;
        int eq_left = (a->left != NULL) ? a->left->eq : 0;
        int eq_right = (a->right != NULL) ? a->right->eq : 0;

        printf("Avant équilibrage - Nœud a : %s, eq_a : %d, eq_left : %d, eq_right : %d\n", a->nomVille, eq_a, eq_left, eq_right);

        if (eq_a >= 2)
        {
            if (eq_right >= 0)
            {
                a = rotationGauche(a);
            }
            else
            {
                a = rotationDoubleGauche(a);
            }
        }
        else if (eq_a <= -2)
        {
            if (eq_left <= 0)
            {
                a = rotationDroite(a);
            }
            else
            {
                a = rotationDoubleDroit(a);
            }
        }

        // Mettre à jour le champ eq après l'équilibrage
        int new_eq = fmax((a->left != NULL) ? a->left->eq : 0, (a->right != NULL) ? a->right->eq : 0) + 1;
        a->eq = new_eq;

        printf("Après équilibrage - Nœud a : %s, eq_a : %d\n", a->nomVille, a->eq);

        return a;
    }

    return a;
}

//


// Fonction pour libérer la mémoire occupée par les nœuds de l'AVL
void freeTree(Arbre *root) {
  if (root == NULL)
  {
     return;
  }
  
  freeTree(root->left);
  freeTree(root->right);
  free(root);
}