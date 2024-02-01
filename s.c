#include"header.h"
#include<math.h>
#include <unistd.h>
#define MAX 50

int max(int a, int b) {
    return a > b ? a : b;
}

int min(int a, int b) {
    return a <= b ? a : b;
}

float moyenne(AVL* tr) {
    if (tr == NULL)
        return 0.0;

    
    float moy = tr->valeur;

    moy += moyenne(tr->fg);
    moy += moyenne(tr->fd);

    return moy;
}
AVL* creerAVL(float x){
  //Fonction qui prend en paramètre un entier et renvoie un AVL
  AVL *a=malloc(sizeof(AVL));
  a->fg=NULL;
  a->fd=NULL;
  a->equilibre=0;
  a->valeur=x;
  return a;
}

AVL* rotationGauche(AVL* a){
  //Fonction qui prend en paramètre un AVL et effectue une rotation gauche
  AVL* pivot;
  int eq_a, eq_p;
  pivot=a->fd;
  a->fd=pivot->fg;
  pivot->fg=a;
  eq_a=a->equilibre;
  eq_p=pivot->equilibre;
  a->equilibre=eq_a-max(eq_p,0)-1;
  pivot->equilibre=min(eq_a-2,eq_a+eq_p-2);
  pivot->equilibre=min(pivot->equilibre,eq_p-1);
  a=pivot;
  return a;
}
AVL* rotationDroite(AVL* a){
  //Fonction qui prend en paramètre un AVL et effectue une rotation droite
  AVL* pivot;
  int eq_a, eq_p;
  pivot=a->fg;
  a->fg=pivot->fd;
  pivot->fd=a;
  eq_a=a->equilibre;
  eq_p=pivot->equilibre;
  a->equilibre=eq_a-min(eq_p,0)+1;
  pivot->equilibre=max(eq_a+2,eq_a+eq_p+2);
  pivot->equilibre=max(pivot->equilibre,eq_p+1);
  a=pivot;
  return a;
}
AVL* doubleRotationGauche(AVL* a){
  //Fonction qui prend en paramètre un AVL et effectue une double rotation gauche
  a->fd=rotationDroite(a->fd);
  return rotationGauche(a);
}
AVL* doubleRotationDroite(AVL* a){
  //Fonction qui prend en paramètre un AVL et effectue une double rotatiob droite
  a->fg=rotationGauche(a->fg);
  return rotationDroite(a);
}
AVL* equilibrage(AVL* a){
  //Fonction qui prend en paramètre un AVL et qui equilibre si besoin cet AVL
  if(a->equilibre>=2){
    if(a->fd->equilibre>=0){
      return rotationGauche(a);
    }
    else{
      return doubleRotationGauche(a);
    }
  }
  else if(a->equilibre<=-2){
    if(a->fg->equilibre<=0){
      return rotationDroite(a);
    }
    else{
      return doubleRotationDroite(a);
    }
  }
  return a;
}
AVL* insertionAVL(AVL* a,float e,int* h){
  //Fonction qui prend en paramètres un AVL un entier et un pointeur sur un entier, qui insert un element dans l'AVL passé en parametre et renvoie l'AVL
  if(a==NULL){
    *h=1;
    return creerAVL(e);
  }
  else if(e<a->valeur){
    a->fg=insertionAVL(a->fg, e, h);
    *h=-*h;
  }
  else if(e>a->valeur){
    a->fd=insertionAVL(a->fd, e, h);
  }
  else{
    *h=0;
    return a;
  }
  if(*h!=0){
    a->equilibre=a->equilibre+*h;
    a=equilibrage(a);
    if(a->equilibre==0){
      *h=0;
    }
    else{
      *h=1;
    }
  }
  return a;
}

int main(int argc, char *argv[]){
  	 FILE *data = fopen("trajet.txt", "rwx");
  	 if (data == NULL) {
         	perror("Erreur lors de l'ouverture du fichier");
        	return 1;
    }
    	 char ligne[256];
    	 float tab[MAX];
    	 int nb = 0;
    	  while (fgets(ligne, sizeof(ligne), data) != NULL) {
    	  	tab[nb] = atof(ligne);
    	  	nb++;
    	  }
          int h = 0;
          float maxAVL, minAVL, moyenneAVL;
          AVL* tr = creerAVL(tab[0]);
          for ( int i = 1; i < nb; i++) {
          	tr = insertionAVL(tr, tab[i], &h);
          }
          AVL* tmp1 = tr;
          AVL* tmp2 = tr;
          while(tmp1->fg != NULL)
          	tmp1 = tmp1->fg;
          
          minAVL = tmp1->valeur;
          while(tmp2->fd != NULL)
          	tmp2 = tmp2->fd;
          	
          maxAVL = tmp2->valeur;
          
          moyenneAVL = moyenne(tr);
	  
	  FILE* fichierTemp = fopen("valeurs_temp.txt", "w");
    	  if (fichierTemp == NULL) {
          	perror("Erreur lors de la création du fichier temporaire");
          	exit(EXIT_FAILURE);
      	  }
      	  fprintf(fichierTemp, "%.2f %.2f %.2f", minAVL, maxAVL, moyenneAVL);
          fclose(fichierTemp);
    	  fclose(data);

    return 0;
  	
}
