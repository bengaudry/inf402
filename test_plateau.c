#include <stdlib.h>
#include <stdio.h>
#include "plateau.h"

int main (int argc, char **argv) {
    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_plateau>\n", argv[0]);
        exit(1);
    }

    Plateau P;

    ErreurPlateau err = lire_fichier_plateau(argv[1], &P);
    if (err != OK) erreur_plateau(err);
    afficher_plateau(P);

    return 0;
}