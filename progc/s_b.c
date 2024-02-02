#include <stdio.h>
#include <stdlib.h>

typedef struct Distance {
    float distance;
    struct Distance* pNext;
} Distance;

typedef struct AVL {
    int routeid;
    Distance* distance;
    struct AVL* pL;
    struct AVL* pR;
    int taille;
    float minDistance;
    float maxDistance;
    float moyenne;
    float moyennedistance;
    int nombredist;
    float sommedist;
} AVL;

Distance* Newdistance(float dist) {
    Distance* p = malloc(sizeof(Distance));
    if (p == NULL) {
        exit(1);
    }
    p->distance = dist;
    p->pNext = NULL;
    return p;
}

AVL* createAVL(int routeNbr, float dist) {
    AVL* pNew = malloc(sizeof(AVL));
    if (pNew == NULL) {
        exit(2);
    }
    pNew->routeid = routeNbr;
    pNew->distance = Newdistance(dist);
    pNew->pL = NULL;
    pNew->pR = NULL;
    pNew->taille = 1;
    pNew->minDistance = dist;
    pNew->maxDistance = dist;
    pNew->moyenne = 0;
    pNew->moyennedistance = dist;
    pNew->nombredist = 1;
    pNew->sommedist = dist;
    return pNew;
}

AVL* createAVL2(int routeNbr, float maxdist, float mindist, float moyenne, float moyennedist) {
    AVL* pNew = malloc(sizeof(AVL));
    if (pNew == NULL) {
        exit(2);
    }
    pNew->routeid = routeNbr;
    pNew->pL = NULL;
    pNew->pR = NULL;
    pNew->taille = 1;
    pNew->minDistance = mindist;
    pNew->maxDistance = maxdist;
    pNew->moyenne = moyenne;
    pNew->moyennedistance = moyennedist;
    return pNew;
}

int taille(AVL* p) {
    if (p == NULL) {
        return 0;
    }
    return p->taille;
}

int getEquilibre(AVL* p) {
    if (p == NULL) {
        return 0;
    }
    return taille(p->pL) - taille(p->pR);
}

AVL* rotationDroite(AVL* p) {
    AVL* p1 = p->pL;
    AVL* p2 = p1->pR;
    p1->pR = p;
    p->pL = p2;
    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));
    p1->taille = 1 + ((taille(p1->pL) > taille(p1->pR)) ? taille(p1->pL) : taille(p1->pR));
    return p1;
}

AVL* rotationGauche(AVL* p) {
    AVL* p1 = p->pR;
    AVL* p2 = p1->pL;
    p1->pL = p;
    p->pR = p2;
    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));
    p1->taille = 1 + ((taille(p1->pL) > taille(p1->pR)) ? taille(p1->pL) : taille(p1->pR));
    return p1;
}

AVL* insertAVL(AVL* p, int routenbr, float dist) {
    if (p == NULL) {
        return createAVL(routenbr, dist);
    }
    if (routenbr < p->routeid) {
        p->pL = insertAVL(p->pL, routenbr, dist);
    } else if (routenbr > p->routeid) {
        p->pR = insertAVL(p->pR, routenbr, dist);
    } else {
        Distance* pNew = Newdistance(dist);
        pNew->distance = p->distance->distance;
        p->distance = pNew;
        p->minDistance = (p->minDistance < dist) ? p->minDistance : dist;
        p->maxDistance = (p->maxDistance > dist) ? p->maxDistance : dist;
        p->moyenne = p->maxDistance - p->minDistance;
        p->sommedist += dist;
        p->nombredist++;
        p->moyennedistance = p->sommedist / p->nombredist;
    }
    p->taille = 1 + ((taille(p->pL) > taille(p->pR)) ? taille(p->pL) : taille(p->pR));
    int balance = getEquilibre(p);
    if (balance > 1 && routenbr < p->pL->routeid) {
        return rotationDroite(p);
    }
    if (balance < -1 && routenbr > p->pR->routeid) {
        return rotationGauche(p);
    }
    if (balance > 1 && routenbr > p->pL->routeid) {
        p->pL = rotationGauche(p->pL);
        return rotationDroite(p);
    }
    if (balance < -1 && routenbr < p->pR->routeid) {
        p->pR = rotationDroite(p->pR);
        return rotationGauche(p);
    }
    return p;
}

AVL* insertDistance(AVL* p2, int numid, float maxdist, float mindist, float moyenne, float moyennedistance) {
    if (p2 == NULL) {
        return createAVL2(numid, maxdist, mindist, moyenne, moyennedistance);
    }
    if (moyenne < p2->moyenne) {
        p2->pL = insertDistance(p2->pL, numid, maxdist, mindist, moyenne, moyennedistance);
    } else if (moyenne > p2->moyenne) {
        p2->pR = insertDistance(p2->pR, numid, maxdist, mindist, moyenne, moyennedistance);
    }
    p2->taille = 1 + ((taille(p2->pL) > taille(p2->pR)) ? taille(p2->pL) : taille(p2->pR));
    int balance = getEquilibre(p2);
    if (balance > 1 && moyenne < p2->pL->moyenne) {
        return rotationDroite(p2);
    }
    if (balance < -1 && moyenne > p2->pR->moyenne) {
        return rotationGauche(p2);
    }
    if (balance > 1 && moyenne > p2->pL->moyenne) {
        p2->pL = rotationGauche(p2->pL);
        return rotationDroite(p2);
    }
    if (balance < -1 && moyenne < p2->pR->moyenne) {
        p2->pR = rotationDroite(p2->pR);
        return rotationGauche(p2);
    }
    return p2;
}

AVL* insertAVL2(AVL* p, AVL* p2) {
    if (p == NULL) {
        return p2;
    }

    // Insérer le nœud courant dans l'arbre résultant
    p2 = insertDistance(p2, p->routeid, p->maxDistance, p->minDistance, p->moyenne, p->moyennedistance);

    // Récursivement insérer les nœuds du sous-arbre gauche
    p2 = insertAVL2(p->pL, p2);

    // Récursivement insérer les nœuds du sous-arbre droit
    p2 = insertAVL2(p->pR, p2);

    // Mettre à jour les statistiques de l'arbre résultant en fonction des sous-arbres fusionnés
    p2->taille = 1 + taille(p2->pL) + taille(p2->pR);
    p2->minDistance = (p2->minDistance < p->minDistance) ? p2->minDistance : p->minDistance;
    p2->maxDistance = (p2->maxDistance > p->maxDistance) ? p2->maxDistance : p->maxDistance;
    p2->moyenne = p2->maxDistance - p2->minDistance;
    p2->sommedist += p->sommedist;
    p2->nombredist += p->nombredist;
    p2->moyennedistance = p2->sommedist / p2->nombredist;

    return p2;
}



void displayInfixeInv(AVL* p) {
    if (p != NULL) {
        displayInfixeInv(p->pR);
        printf("%d;%f;%f;%f;%f\n", p->routeid, p->minDistance, p->moyennedistance, p->maxDistance, p->moyenne);
        displayInfixeInv(p->pL);
    }
}

void destroyAVL(AVL* p) {
    if (p != NULL) {
        destroyAVL(p->pL);
        destroyAVL(p->pR);
        while (p->distance != NULL) {
            Distance* temp = p->distance->pNext;
            free(p->distance);
            p->distance = temp;
        }
        free(p);
    }
}

int main() {
    AVL* AVL1 = NULL;
    AVL* AVL2 = NULL;
    float dist;
    int routeid;
    while (scanf("%d;%f\n", &routeid, &dist) == 2) {
        AVL1 = insertAVL(AVL1, routeid, dist);
    }
    AVL2 = insertAVL2(AVL1, AVL2);
    displayInfixeInv(AVL2);
    destroyAVL(AVL2);  // Ajout de la libération de mémoire
    return 0;
}