#include "AVL_t.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    fprintf(stderr, "Usage: %s nom_du_fichier_entree\n", argv[0]);
    return EXIT_FAILURE;
  }

  char *fichier_entree = argv[1];

  // On effectue le traitement directement dans la fonction
  traitement_t(fichier_entree);

  printf("Traitement t effectué avec succès.\n");

  return EXIT_SUCCESS;
}
