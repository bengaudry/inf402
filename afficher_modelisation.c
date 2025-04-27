#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"

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

    fnc = modeliser_jeu(P);
    printf("> Modélisation :\n");
    afficher_FNC(*fnc);
    printf("\n> STATS\n");
    printf("- %d variables logiques\n", fnc->nb_variables);
    printf("- %d clauses\n\n", fnc->taille);
    sortie_dimacs(*fnc, 0, 0, "sat.dimacs");
    
    printf("> Modélisation en 3-SAT :\n");
    fnc = sat_vers_3sat(fnc);
    afficher_FNC(*fnc);
    printf("\n> STATS 3-SAT\n");
    printf("- %d variables logiques\n", fnc->nb_variables);
    printf("- %d clauses\n", fnc->taille);
    sortie_dimacs(*fnc, 0, 0, "3sat.dimacs");

    return 0;
}
