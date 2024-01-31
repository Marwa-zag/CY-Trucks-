#include "avl_t_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");//replace by arg

    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        return 1;
    }

    char arg = *argv[2];
    if (arg == 'T') T(file);
    else if (arg == 'S') S(file);
    else abort();

    fclose(file);

    return 0;
}