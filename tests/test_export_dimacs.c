#include <stdlib.h>
#include <stdio.h>

#include "../plateau.h"
#include "../modelisation.h"
#include "../dimacs.h"

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

    printf("\n\n> Total: - %d clauses\n", fnc->taille);
    printf("         - %d variables\n", fnc->nb_variables);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "sat.dimacs");


    printf("\n\n> Transformation en 3-SAT :\n");
    fnc = sat_vers_3sat(fnc);
    afficher_FNC(*fnc);

    printf("\n\n> Total: - %d clauses\n", fnc->taille);
    printf("         - %d variables\n", fnc->nb_variables);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "3sat.dimacs");

    return 0;
}
