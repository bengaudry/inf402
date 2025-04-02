#include <stdlib.h>
#include <stdio.h>

#include "../plateau.h"
#include "../modelisation.h"

int main (int argc, char **argv) {
    Plateau P;
    ErreurPlateau err;

    FNC *fnc;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_plateau>\n", argv[0]);
        exit(1);
    }

    err = lire_fichier_plateau(argv[1], &P);
    if (err != OK) erreur_plateau(err);
    afficher_plateau(P);

    printf("\n\n> Modélisation :\n");
    fnc = modeliser_jeu(P);
    afficher_FNC(*fnc);

    return 0;
}
