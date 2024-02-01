#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "arbre.h"

int main(int argc, char *argv[]) {
    FILE *file = fopen(argv[1], "r");

    if (!file) {
        printf("Impossible d'ouvrir le fichier\n");
        return 1;
    }

    char *line = NULL;
    size_t len = 0;
    size_t read;

    char arg = *argv[2];
    if (arg == 'T') {
        T_Node *root = NULL;

        // Lire chaque ligne du fichier
        while ((read = getline(&line, &len, file)) != -1) {
            line[strcspn(line, "\n")] = 0; // On enlève les \n en fin de chaîne

            char *subString = strtok(line, ";");
            int i = 0;
            while (subString != NULL) {
                if (i == 2) {
                    char *name = malloc(strlen(subString) + 1);
                    strcpy(name, subString);
                    root = TInsertNode(root, name, 1);
                } else if (i == 3) {
                    char *name = malloc(strlen(subString) + 1);
                    strcpy(name, subString);
                    root = TInsertNode(root, name, 0);
                }

                i++;
                subString = strtok(NULL, ";");
            }
        }

        // Rediriger la sortie vers un fichier
        FILE *outputFile = freopen("resultat_T.txt", "w", stdout);

        // Appeler la fonction pour imprimer les valeurs les plus élevées
        TPrintHighestValues(root);

        // Restaurer la sortie standard
        freopen("/dev/tty", "a", stdout);

        // Afficher un message dans le terminal
        printf("Les résultats de T ont été enregistrés avec succès dans le fichier resultat_T.txt.\n");

        // Fermer le fichier
        fclose(outputFile);
    } else if (arg == 'S') {
        S_Node *root = NULL;

        // Lire chaque ligne du fichier
        while ((read = getline(&line, &len, file)) != -1) {
            line[strcspn(line, "\n")] = 0; // On enlève les \n en fin de chaîne

            char *subString = strtok(line, ";");
            int i = 0;
            int id = -1;
            double dst = -1;
            while (subString != NULL) {
                if (i == 0) {
                    id = atoi(subString);
                } else if (i == 4) {
                    char *_;
                    dst = strtod(subString, &_);
                } else if (i == 5) {
                    root = SInsertNode(root, id, dst);
                }

                i++;
                subString = strtok(NULL, ";");
            }
        }

        // Rediriger la sortie vers un fichier
        FILE *outputFile = freopen("resultat_S.txt", "w", stdout);

        // Appeler la fonction pour imprimer les valeurs les plus élevées
        SPrintHighestValues(root);

        // Restaurer la sortie standard
        freopen("/dev/tty", "a", stdout);

        // Afficher un message dans le terminal
        printf("Les résultats de S ont été enregistrés avec succès dans le fichier resultat_S.txt.\n");

        // Fermer le fichier
        fclose(outputFile);
    } else {
        abort();
    }

    fclose(file);
    if (line) {
        free(line);
    }

    return 0;
}
