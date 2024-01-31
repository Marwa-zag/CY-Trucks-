#include "avl_t_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Ville *CreerVille(char *nom) {
  Ville *ville = malloc(sizeof(Ville));
  ville->nom = nom;
  ville->count = 1;
  ville->count_depart = 0;

  return ville;
}

int T_Height(Noeud_Arbre *N) { return N == NULL ? 0 : N->hauteur; }

int S_Height(Noeud_Structure *N) { return N == NULL ? 0 : N->hauteur; }

int max(int a, int b) { return (a > b) ? a : b; }

Noeud_Arbre *MakeTNode(char *nom) {
  Noeud_Arbre *node = malloc(sizeof(Noeud_Arbre));
  node->cle = CreerVille(nom);
  node->gauche = NULL;
  node->droite = NULL;
  node->hauteur = 1;
  return node;
}

Noeud_Structure *MakeSNode(int id) {
  Noeud_Structure *node = malloc(sizeof(Noeud_Structure));
  node->id_trajet = id;
  node->distances = malloc(sizeof(Liste_Liee));
  node->gauche = NULL;
  node->droite = NULL;
  node->hauteur = 1;
  return node;
}

Noeud_Arbre *TRotateRight(Noeud_Arbre *y) {
  if (y->gauche == NULL)
    return NULL;

  Noeud_Arbre *x = y->gauche;
  Noeud_Arbre *T2 = x->droite;

  x->droite = y;
  y->gauche = T2;

  y->hauteur = max(T_Height(y->gauche), T_Height(y->droite)) + 1;
  x->hauteur = max(T_Height(x->gauche), T_Height(x->droite)) + 1;

  return x;
}

Noeud_Arbre *TRotateLeft(Noeud_Arbre *x) {
  if (x->droite == NULL)
    return NULL;

  Noeud_Arbre *y = x->droite;
  Noeud_Arbre *T2 = y->gauche;

  y->gauche = x;
  x->droite = T2;

  x->hauteur = max(T_Height(x->gauche), T_Height(x->droite)) + 1;
  y->hauteur = max(T_Height(y->gauche), T_Height(y->droite)) + 1;

  return y;
}

Noeud_Structure *SRotateRight(Noeud_Structure *y) {
  if (y->gauche == NULL)
    return NULL;

  Noeud_Structure *x = y->gauche;
  Noeud_Structure *T2 = x->droite;

  x->droite = y;
  y->gauche = T2;

  y->hauteur = max(S_Height(y->gauche), S_Height(y->droite)) + 1;
  x->hauteur = max(S_Height(x->gauche), S_Height(x->droite)) + 1;

  return x;
}

Noeud_Structure *SRotateLeft(Noeud_Structure *x) {
  if (x->droite == NULL)
    return NULL;

  Noeud_Structure *y = x->droite;
  Noeud_Structure *T2 = y->gauche;

  y->gauche = x;
  x->droite = T2;

  x->hauteur = max(S_Height(x->gauche), S_Height(x->droite)) + 1;
  y->hauteur = max(S_Height(y->gauche), S_Height(y->droite)) + 1;

  return y;
}

int TGetBalance(Noeud_Arbre *N) {
  return N == NULL ? 0 : T_Height(N->gauche) - T_Height(N->droite);
}

int SGetBalance(Noeud_Structure *N) {
  return N == NULL ? 0 : T_Height(N->gauche) - T_Height(N->droite);
}

Noeud_Arbre *TInsertNode(Noeud_Arbre *node, char *nom, int depart) {
  if (node == NULL)
    return (MakeTNode(nom));

  int cmp = strcmp(node->cle->nom, nom);
  if (cmp < 0)
    node->gauche = TInsertNode(node->gauche, nom, depart);
  else if (cmp > 0)
    node->droite = TInsertNode(node->droite, nom, depart);
  else {
    if (depart)
      node->cle->count_depart++;
    node->cle->count++;
    return node;
  }

  // Update the balance factor of each node and
  // Balance the tree
  node->hauteur = 1 + max(T_Height(node->gauche), T_Height(node->droite));

  int balance = TGetBalance(node);

  int left_cmp = node->gauche == NULL ? 0 : strcmp(node->gauche->cle->nom, nom);
  if (balance > 1 && left_cmp < 0)
    return TRotateRight(node);

  int right_cmp =
      node->droite == NULL ? 0 : strcmp(node->droite->cle->nom, nom);
  if (balance < -1 && right_cmp > 0)
    return TRotateLeft(node);

  if (balance > 1 && left_cmp > 0) {
    node->gauche = TRotateLeft(node->gauche);
    return TRotateRight(node);
  }

  if (balance < -1 && right_cmp < 0) {
    node->droite = TRotateRight(node->droite);
    return TRotateLeft(node);
  }

  return node;
}

void AddToLList(Liste_Liee *liste, double dst) {
  if (liste->valeur == 0) {
    liste->valeur = dst;
    return;
  }

  Liste_Liee *suivant = liste;
  while (suivant->suivant != NULL) {
    suivant = suivant->suivant;
  }

  Liste_Liee *new = malloc(sizeof(Liste_Liee));
  new->valeur = dst;

  suivant->suivant = new;
}

Noeud_Structure *SInsertNode(Noeud_Structure *node, int id, double dst) {
  if (node == NULL)
    return (MakeSNode(id));

  if (id < node->id_trajet)
    node->gauche = SInsertNode(node->gauche, id, dst);
  else if (id > node->id_trajet)
    node->droite = SInsertNode(node->droite, id, dst);
  else {
    AddToLList(node->distances, dst);
    return node;
  }

  // Update the balance factor of each node and
  // Balance the tree
  node->hauteur = 1 + max(S_Height(node->gauche), S_Height(node->droite));

  int balance = SGetBalance(node);

  if (balance > 1 && id < node->gauche->id_trajet)
    return SRotateRight(node);

  if (balance < -1 && id > node->droite->id_trajet)
    return SRotateLeft(node);

  if (balance > 1 && id > node->gauche->id_trajet) {
    node->gauche = SRotateLeft(node->gauche);
    return SRotateRight(node);
  }

  if (balance < -1 && id > node->droite->id_trajet) {
    node->droite = SRotateRight(node->droite);
    return SRotateLeft(node);
  }

  return node;
}

void TInsertNodeOrderedList(Noeud_Arbre *node, Noeud_Arbre **nodes) {
  // La liste est triée décroissante
  int i = 0;
  for (; i < 10; ++i)
    if (nodes[i] == NULL || node->cle->count > nodes[i]->cle->count)
      break;
  if (i == 10)
    return; // Aucune valeur à ajouter

  Noeud_Arbre *precedent = node;
  Noeud_Arbre *tmp;
  for (; i < 10 && precedent != NULL; ++i) {
    tmp = nodes[i];
    nodes[i] = precedent;
    precedent = tmp;
  }
}

struct Info_Liste_Liee GetInfoListeL(LList *liste) {
  struct Info_Liste_Liee info;
  info.min = liste->valeur;
  info.max = liste->valeur;

  int count = 0;
  double somme = 0;
  Liste_Liee *suivant = liste;
  while (suivant != NULL) {
    if (suivant->valeur < info.min)
      info.min = suivant->valeur;
    else if (suivant->valeur > info.max)
      info.max = suivant->valeur;

    somme += suivant->valeur;
    count++;
    suivant = suivant->suivant;
  }

  info.moyenne = somme / count;
  return info;
}

void SInsertNodeOrderedList(Noeud_Structure *node, Noeud_Structure **nodes) {
  // La liste est triée décroissante
  int i = 0;
  for (; i < 50; ++i) {
    if (nodes[i] == NULL)
      break;
    struct Info_Liste_Liee info = GetInfoListeL(node->distances);
    struct Info_Liste_Liee info2 = GetInfoListeL(nodes[i]->distances);
    if (info.max - info.min > info2.max - info.min)
      break;
  }
  if (i == 50)
    return; // Aucune valeur à ajouter

  Noeud_Structure *precedent = node;
  Noeud_Structure *tmp;
  for (; i < 50 && precedent != NULL; ++i) {
    tmp = nodes[i];
    nodes[i] = precedent;
    precedent = tmp;
  }
}

void RechercherDixPlusHautesValeurs(Noeud_Arbre *racine, Noeud_Arbre **nodes) {
  if (racine == NULL)
    return;

  TInsertNodeOrderedList(racine, nodes);

  RechercherDixPlusHautesValeurs(racine->gauche, nodes);
  RechercherDixPlusHautesValeurs(racine->droite, nodes);
}

void TImprimerPlusHautesValeurs(Noeud_Arbre *racine) {
  if (racine == NULL)
    return;

  Noeud_Arbre **nodes = malloc(10 * sizeof(Noeud_Arbre *));
  RechercherDixPlusHautesValeurs(racine, nodes);
  for (int i = 0; i < 10; ++i) {
    printf("%d | %s - %d - %d\n", i + 1, nodes[i]->cle->nom,
           nodes[i]->cle->count, nodes[i]->cle->count_depart);
  }
}

void RechercherCinquantePlusHautesValeurs(Noeud_Structure *racine,
                                          Noeud_Structure **nodes) {
  if (racine == NULL)
    return;

  SInsertNodeOrderedList(racine, nodes);

  RechercherCinquantePlusHautesValeurs(racine->gauche, nodes);
  RechercherCinquantePlusHautesValeurs(racine->droite, nodes);
}

void SImprimerPlusHautesValeurs(Noeud_Structure *racine) {
  if (racine == NULL)
    return;

  Noeud_Structure **nodes = malloc(50 * sizeof(Noeud_Structure *));
  RechercherCinquantePlusHautesValeurs(racine, nodes);
  for (int i = 0; i < 50; ++i) {
    struct Info_Liste_Liee info = GetInfoListeL(nodes[i]->distances);
    printf("%d | %d - %f - %f - %f\n", i + 1, nodes[i]->id_trajet, info.min,
           info.moyenne, info.max);
  }
}

Noeud_Arbre **ObtenirDixVillesLesPlusNombreuses(Noeud_Arbre *racine) {
  Noeud_Arbre **nodes = malloc(10 * sizeof(Noeud_Arbre *));
  RechercherDixPlusHautesValeurs(racine, nodes);
  return nodes;
}

void ImprimerPreOrdre(Noeud_Arbre *racine) {
  if (racine != NULL) {
    printf("%s : %d\n", racine->cle->nom, racine->cle->count);
    ImprimerPreOrdre(racine->gauche);
    ImprimerPreOrdre(racine->droite);
  }
}

Noeud_Arbre *T(FILE *fichier) {
  Noeud_Arbre *racine = NULL;

  char buffer[1024];
  int numero_ligne = 1;
  fgets(buffer, 1024, fichier);
  while (fgets(buffer, 1024, fichier)) {
    buffer[strcspn(buffer, "\n")] = 0; // On enlève les \n en fin de chaîne

    char *sousChaine = strtok(buffer, ";");
    int i = 0;
    while (sousChaine != NULL) {
      if (i == 2) {
        char *nom = malloc(32);
        strcpy(nom, sousChaine);
        racine = TInsertNode(racine, nom, 1);
      } else if (i == 3) {
        char *nom = malloc(32);
        strcpy(nom, sousChaine);
        racine = TInsertNode(racine, nom, 0);
      }

      i++;
      sousChaine = strtok(NULL, ";");
    }
    numero_ligne++;
  }

  TImprimerPlusHautesValeurs(racine);
}

Noeud_Structure *S(FILE *fichier) {
  Noeud_Structure *racine = NULL;

  char buffer[1024];
  int numero_ligne = 1;
  fgets(buffer, 1024, fichier);
  while (fgets(buffer, 1024, fichier)) {
    buffer[strcspn(buffer, "\n")] = 0; // On enlève les \n en fin de chaîne

    char *sousChaine = strtok(buffer, ";");
    int i = 0;
    int id = -1;
    double dst = -1;
    while (sousChaine != NULL) {
      if (i == 0) {
        id = atoi(sousChaine);
      } else if (i == 4) {
        char *_;
        dst = strtod(sousChaine, &_);
      } else if (i == 5) {
        racine = SInsertNode(racine, id, dst);
      }

      i++;
      sousChaine = strtok(NULL, ";");
    }
    numero_ligne++;
  }

  SImprimerPlusHautesValeurs(racine);
}
