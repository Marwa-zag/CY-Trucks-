#ifndef AVL_T_H
#define AVL_T_H

// AVL
typedef struct _avl_t {
  char nomVille[100]; // nom ville
  int nbrTrajets;    // nombre d'itération de la ville
  int nbrDepart;     // nombre de fois ou la ville est départ
  struct _avl_t *left;
  struct _avl_t *right;
  int eq; // equilibre
} Arbre;

// Outil_AVL_T -> fonct° + struct AVL pour traitement T
typedef struct VilleStats {
  char nomVille[100];
  int nbTrajets;
  int nbDepart;
} Ville;
//--------------------- TRAITEMENT T ---------------------//

void traitement_t(char *fichier);
void afficherArbre(Arbre *node);
Arbre* creerArbre(char* nomVille);
void postfixeFilsDroit(Arbre* node, Ville tableau[], int *i) ;
Arbre* insertion(Arbre* node, char* nomVille, int* h, int depart);
Arbre* rotationGauche(Arbre* a);
Arbre* rotationDroite(Arbre* a);
Arbre* rotationDoubleGauche(Arbre* a);
Arbre* rotationDoubleDroit(Arbre* a);
Arbre* equilibrageAVL(Arbre* a);
void freeTree(Arbre *root);

#endif