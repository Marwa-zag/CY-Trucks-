#include<stdio.h>
#include <stdlib.h>

// Structure pour un nœud d'Arbre Binaire de Recherche (ABR)
typedef struct ABR{
  int route_ID;// Identifiant de la route
  int etape_ID;// Identifiant de l'étape
  float distance;// Distance parcourue
  float min;// Distance minimale
  float max;// Distance maximale
  int n;// Nombre d'occurrences
  struct ABR* fG;// Fils gauche
  struct ABR* fD;// Fils droit
}ABR;


// Structure pour un nœud d'Arbre AVL
typedef struct AVL{
    int route_ID;// Identifiant de la route
    float min;// Distance minimale
    float max;// Distance maximale
    float moy;// Distance moyenne
    float diff;// Différence entre max et min
    int eq;// Facteur d'équilibre
    struct AVL* fG;// Fils gauche
    struct AVL* fD;// Fils droit
}AVL;


// Crée un nouveau nœud ABR
ABR* creationNoeudABR(int route_ID, int etape_ID, float distance, float min, float max){
    // Allocation de mémoire pour un nouveau nœud ABR
    ABR* pNew = malloc(sizeof(ABR));
    
    if (pNew == NULL) {
        printf("Erreur au niveau du malloc");
        exit(2);
    }

    // Initialisation des valeurs du nœud
    pNew->route_ID = route_ID;
    pNew->etape_ID = etape_ID;
    pNew->distance = distance;
    pNew->min = min;
    pNew->max = max;
    pNew->n = 1 ;
    pNew->fG = NULL;
    pNew->fD = NULL;
    return pNew;
}


// Insère un élément dans l'ABR
ABR* insertionABR(ABR* abr, int route_ID, int etape_ID, float distance){
    // Insertion récursive dans l'arbre
    if(abr == NULL){
        abr = creationNoeudABR(route_ID, etape_ID, distance, distance, distance);
    }
    else if(route_ID < abr->route_ID){
        abr->fG = insertionABR(abr->fG, route_ID, etape_ID, distance);
    } 
    else if(route_ID > abr->route_ID){
        abr->fD = insertionABR(abr->fD, route_ID, etape_ID, distance);
    }
    else if(route_ID == abr->route_ID){
        // Mise à jour des valeurs si l'ID de route est identique
        abr->n++;
        abr->distance += distance;
        if(distance > abr->max){
            abr->max = distance;
        }

        if(distance < abr->min){
            abr->min = distance;
        }
    }

    return abr;
}

// Lit un fichier CSV et remplit l'ABR avec les données
ABR* lectureCSV(const char* data, ABR* abr) {
    FILE* fichier = fopen( data, "r");
    if (fichier == NULL) {
        perror("Erreur dans l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int route_ID, etape_ID;
    float distance;
    
    while(fscanf(fichier, "%d;%d;%f", &route_ID, &etape_ID, &distance) == 3) {
        
        abr = insertionABR(abr, route_ID, etape_ID, distance);//On remplis l'arbre avec les données du fichier
    }
    return abr;

    fclose(fichier);
}

// Fonction d'inversion pour l'écriture dans le fichier en ordre décroissant
void inversion(AVL* avl, FILE* fichier2) {
    if (avl != NULL) {
        inversion(avl->fD, fichier2);// Parcours du sous-arbre droit
        // Écriture des données de l'AVL dans le fichier
        
        fprintf(fichier2, "%d;%.3f;%.3f;%.3f;%.3f\n", avl->route_ID, avl->min, avl->moy, avl->max, avl->diff);
        inversion(avl->fG, fichier2);// Parcours du sous-arbre gauche
    }
}


// Parcourt l'ABR en ordre et affiche les valeurs (pour test)
void infixtestABR(ABR* p){
    printf("ABR\n");
    if(p != NULL){
        
    infixtestABR(p->fD);
    printf("[%02d], %d;%.3f;%.3f;%.3f;%d", p->route_ID, p->etape_ID, p->min, p->max, p->distance, p->n);
    
    infixtestABR(p->fG);
    }
}

// Parcourt l'AVL en ordre et affiche les valeurs (pour test)
void infixtestAVL(AVL* p){
    printf("AVL\n");
    if(p != NULL){
        
    infixtestAVL(p->fD);
    printf("[%02d]", p->route_ID);
    
    infixtestAVL(p->fG);
    }
}

// Crée un nouveau nœud AVL à partir d'un nœud ABR
AVL* creationNoeudAVL(ABR* abr){
    
    AVL* new = malloc(sizeof(AVL));
    if (new == NULL) {
        printf("Erreur au niveau du malloc");
        exit(3);
    }
    
    // Calcul des valeurs pour le nœud AVL
    new->route_ID = abr->route_ID;
    new->min = abr->min;
    new->max = abr->max;
    new->moy = (abr->distance / (double)abr->n);
    new->diff = abr->max - abr->min;
    new->eq = 0;
    new->fG = NULL;
    new->fD = NULL;

    return new;
}


//Fonction qui retourne le minimum entre trois entiers 
int min3(int a, int b, int c){
return (a < b) ? ((a < c) ? a : c) : ((b < c) ? b : c);
}

//Fonction qui retourne le max entre deux entiers 
int max2(int a, int b){
    return (a > b) ? a : b;
}

//Fonction rotation gauche (cours)
AVL* RotationG(AVL* avl){
    if(avl == NULL){
        printf("L'arbre est null\n");
        exit(4);
    }
    AVL* pivot;
    int eq_a, eq_p;

    pivot = avl->fD;
    avl->fD = pivot->fG;
    pivot->fG = avl;
    
    eq_a = avl->eq;
    eq_p = pivot->eq;
    avl->eq = eq_a - max2(eq_p, 0) - 1;
    pivot->eq = min3(eq_a - 2, eq_a + eq_p - 2, eq_p-1);

    avl = pivot;

    return avl;
}

//Fonction qui retourne le minimum entre deux entiers
int min2(int a, int b) {
    return (a < b) ? a : b;
}

//Fonction qui retourne le maximum entre trois entiers
int max3(int a, int b, int c) {
    return (a > b) ? ((a > c) ? a : c) : ((b > c) ? b : c);
}

//Fonction rotation droite 
AVL* RotationD(AVL* avl){
    if(avl == NULL){
        printf("L'arbre est null\n");
        exit(5);
    }
    AVL* pivot;
    int eq_a, eq_p;

    pivot = avl->fG;
    avl->fG = pivot->fD;
    pivot->fD = avl;
    
    eq_a = avl->eq;
    eq_p = pivot->eq;
    avl->eq = eq_a - min2(eq_p, 0) + 1;
    pivot->eq = max3(eq_a + 2, eq_a + eq_p + 2, eq_p + 1);

    avl = pivot;

    return avl;
}

//Focnction double rotation gauche 
AVL* doubleRotationG(AVL* avl){
    if(avl == NULL){
        printf("L'arbre est null\n");
        exit(6);
    }
    avl->fD = RotationD(avl->fD);
    return RotationG(avl);
}

//Fonction double rotation droite 
AVL* doubleRotationD(AVL* avl){
        if(avl == NULL){
        printf("L'arbre est null\n");
        exit(7);
    }
    avl->fG = RotationG(avl->fG);
    return RotationD(avl);
}

//Fonction pour equilibre un AVL 
AVL* equilibrageAVL(AVL* avl){
        if(avl == NULL){
        printf("L'arbre est null\n");
        exit(8);
    }
    if(avl->eq > 1){
        if(avl->fD->eq >= 0){
            return RotationG(avl);
        }
        else{
            return doubleRotationG(avl);
        }
    }
    else if(avl->eq < -1){
        if(avl->fG->eq <= 0){
            return RotationD(avl);
        }
        else{
            return doubleRotationD(avl);
        }
    }
    return avl;
}


//Fonction insertion dans un AVL avec l'equilibrage 
AVL* insertionAVL(AVL* avl, int* h, ABR* abr){
    AVL* new = creationNoeudAVL(abr);

    if(avl ==  NULL){
        *h = 1;
        return new;
    }
    else if(new->diff < avl->diff){
        avl->fG = insertionAVL(avl->fG, h, abr);
        *h = -(*h);
    }
    else if(new->diff > avl->diff){
        avl->fD = insertionAVL(avl->fD, h, abr);
    }
    else{
        *h = 0;
        return avl;
    }

    if(*h != 0){
        avl->eq = avl->eq + *h;
        avl = equilibrageAVL(avl);
        if(avl->eq == 0){
            *h = 0;
        }
        else{
            *h = 1;
        }

    }
    return avl;
}


// Parcourt l'ABR et insère les éléments dans l'AVL
AVL* parcoursAVL(ABR* abr, AVL* avl, int h){
    if(abr != NULL){
        //Insère la valeur actuelle de l'ABR dans l'AVL
        avl = insertionAVL(avl, &h, abr);
        
        //Continue le parcours en ordre dans l'ABR
        avl = parcoursAVL(abr->fG, avl, h);
        avl = parcoursAVL(abr->fD, avl, h);
    } 
    return avl;
}


// Libère la mémoire allouée pour l'ABR
void freeABR(ABR* abr) {
    // Si le nœud est NULL, il n'y a rien à libérer, donc return
    if (abr == NULL) {
        return;
    }

    // Récursivement libérer le sous-arbre gauche et le sous-arbre droit
    freeABR(abr->fG);
    freeABR(abr->fD);

    // Libérer le nœud actuel
    free(abr);
}


// Libère la mémoire allouée pour l'AVL
void freeAVL(AVL* avl){
    //si NULL alors return
    if (avl == NULL) {
        return;
    }

    //Libére le sous-arbre gauche et le sous-arbre droit
    freeAVL(avl->fG);
    freeAVL(avl->fD);

    //Libére le noeud sur lequel on se trouve 
    free(avl);
}


// Affiche le contenu d'un fichier dans le terminal
void afficherContenuFichier(const char* nomFichier) {
    FILE* fichier = fopen(nomFichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    int caractere;
    while ((caractere = fgetc(fichier)) != EOF) {
        putchar(caractere);
    }

    fclose(fichier);
}

int main(int argc, char *argv[]) {
    // Vérification des arguments du programme
    if (argc != 2) {
        printf("Il y a plus d'un argument ");
        exit (1);
    }

    // Création et remplissage de l'ABR à partir du fichier CSV
    ABR* abr = NULL;
    ABR* abr = NULL;
    abr = lectureCSV(argv[1], abr);

    // Création de l'AVL
    AVL* avl = NULL;
 
    int h = 0;
    avl = parcoursAVL(abr, avl, h);// Parcours de l'ABR et insertion dans l'AVL

    
    // Écriture des données de l'AVL dans un fichier
    FILE *fichiertemp = fopen("temp/sortie.csv", "w");
    if (fichiertemp == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(EXIT_FAILURE);
    }

    inversion(avl, fichiertemp);
    printf("Fichier temp/sortie.csv créé avec succès.\n");

    fclose(fichiertemp);

    // Affichage du contenu du fichier généré
    printf("Contenu du fichier temp/s.txt :\n");
    afficherContenuFichier("temp/s.txt");

    // Libération de la mémoire
    freeABR(abr);
    freeAVL(avl);

    return 0;
}
