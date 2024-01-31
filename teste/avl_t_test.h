#ifndef avl_t_test_h
#define avl_t_test_h

typedef struct Ville {
  char *nom;
  int count;
  int count_depart;
} Ville;

typedef struct Noeud_Arbre {
  Ville *cle;
  struct Noeud_Arbre *gauche;
  struct Noeud_Arbre *droite;
  int hauteur;
} Noeud_Arbre;

typedef struct Liste_Liee {
  double valeur;
  struct Liste_Liee *suivant;
} Liste_Liee;

typedef struct Noeud_Structure {
  int id_trajet;
  Liste_Liee *distances;

  struct Noeud_Structure *gauche;
  struct Noeud_Structure *droite;
  int hauteur;
} Noeud_Structure;

struct Info_Liste_Liee {
    double min;
    double max;
    double moyenne;
};

int T_Height(Noeud_Arbre *N);
int S_Height(Noeud_Structure *N);
int max(int a, int b);
Noeud_Arbre *MakeTNode(char *nom);
Noeud_Structure *MakeSNode(int id);
Noeud_Arbre *TRotateRight(Noeud_Arbre *y);
Noeud_Arbre *TRotateLeft(Noeud_Arbre *x);
Noeud_Structure *SRotateRight(Noeud_Structure *y);
Noeud_Structure *SRotateLeft(Noeud_Structure *x);
int TGetBalance(Noeud_Arbre *N);
int SGetBalance(Noeud_Structure *N);
Noeud_Arbre *TInsertNode(Noeud_Arbre *node, char *nom, int depart);
void AddToLList(Liste_Liee *liste, double dst);
Noeud_Structure *SInsertNode(Noeud_Structure *node, int id, double dst); 
void TInsertNodeOrderedList(Noeud_Arbre *node, Noeud_Arbre **nodes);
struct Info_Liste_Liee GetInfoListeL(LList *liste);
void SInsertNodeOrderedList(Noeud_Structure *node, Noeud_Structure **nodes);
void RechercherDixPlusHautesValeurs(Noeud_Arbre *racine, Noeud_Arbre **nodes);
void TImprimerPlusHautesValeurs(Noeud_Arbre *racine);
void RechercherCinquantePlusHautesValeurs(Noeud_Structure *racine, Noeud_Structure **nodes);
void SImprimerPlusHautesValeurs(Noeud_Structure *racine);
Noeud_Arbre **ObtenirDixVillesLesPlusNombreuses(Noeud_Arbre *racine);
void ImprimerPreOrdre(Noeud_Arbre *racine);
Noeud_Arbre *T(FILE *fichier);
Noeud_Structure *S(FILE *fichier);

#endif
