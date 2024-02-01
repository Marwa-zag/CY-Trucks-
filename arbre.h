#ifndef ARBRE_H
#define ARBRE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct City {
    char *name;
    int count;
    int departureCount;
} City;

typedef struct T_Node {
    City *key;
    struct T_Node *left;
    struct T_Node *right;
    int height;
} T_Node;

typedef struct LList {
    double value;
    struct LList *next;
} LList;

typedef struct S_Node {
    int routeID;
    LList *distances;

    struct S_Node *left;
    struct S_Node *right;
    int height;
} S_Node;

struct LListInfo {
    double min;
    double max;
    double average;
};

City *MakeCity(char *name);
int T_Height(T_Node *N);
int S_Height(S_Node *N);
int max(int a, int b);
T_Node *MakeTNode(char *name);
S_Node *MakeSNode(int id);
T_Node *TRotateRight(T_Node *y);
T_Node *TRotateLeft(T_Node *x);
S_Node *SRotateRight(S_Node *y);
S_Node *SRotateLeft(S_Node *x);
int TGetBalance(T_Node *N);
int SGetBalance(S_Node *N);
T_Node *TInsertNode(T_Node *node, char *name, int departure);
void AddToLList(LList *list, double dst);
S_Node *SInsertNode(S_Node *node, int id, double dst);
void TInsertNodeOrderedList(T_Node *node, T_Node **nodes);
struct LListInfo GetLListInfo(LList *list);
void SInsertNodeOrderedList(S_Node *node, S_Node **nodes);
void SearchTenHighestValues(T_Node *root, T_Node **nodes);
void TPrintHighestValues(T_Node *root);
void SearchFiftyHighestValues(S_Node *root, S_Node **nodes);
void SPrintHighestValues(S_Node *root);
T_Node **GetTenHighestCountCities(T_Node *root);
void PrintPreOrder(T_Node *root);
T_Node *T(FILE *file);
S_Node *S(FILE *file);


#endif






















