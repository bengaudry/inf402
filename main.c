#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"

#include "sat_solver.h"
#include "minisat.h"

// #define USING_CUSTOM_SOLVER

int main (int argc, char **argv) {
    Plateau P;
    ErreurPlateau err;
    FNC* fnc;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_plateau>\n", argv[0]);
        exit(1);
    }

    err = lire_fichier_plateau(argv[1], &P);
    if (err != OK) erreur_plateau(err);
    afficher_plateau(P);

#ifndef USING_CUSTOM_SOLVER
    if (generer_solution(P, &P)) {
        printf("\n> SOLUTION :\n");
        afficher_plateau(P);
    } else {
        printf("\nPas de solution, ce jeu est insatisfaisable.\n");
    }
#endif

#ifdef USING_CUSTOM_SOLVER
    fnc = sat_vers_3sat(fnc);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "3sat.dimacs");

    printf("\n> Solving ...\n");
    SS_FNC ss_fnc;
    ss_fnc = lire_fichier_dimacs("3sat.dimacs");
    //afficher_fnc(ss_fnc);

    SS_REPONSE rep;
    rep.taille = 0;
    bool succes = solve_3sat(ss_fnc, &rep);

    if (!succes) {
        printf("INSAT\n");
        return 1;
    }

    for (int i = 0;  i <= rep.taille; i++) {
        int k = rep.valeurs[i];
        if (k > 0) { // variable vraie
            printf("%d ", k);
            VarLogique dec = decodage_id(*fnc, k-1);
     		//printf("(%d, %d, %d)\n", dec.val, dec.x, dec.y);
            Case c;
            c.coor = creer_coor(dec.x, dec.y);
            c.type = TypeNombre;
            c.val.nombre = dec.val;
            modifier_case(&P, dec.x, dec.y, c);
        }
    }

    printf("\n\n> SOLUTION\n");
    afficher_plateau(P);
#endif
    return 0;
}
