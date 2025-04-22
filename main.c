#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"

#include "minisat_src/minisat.h"
#include "minisat_src/solver.h"
#include "sat_solver.h"

// #define USING_CUSTOM_SOLVER

int main (int argc, char **argv) {
    Plateau P;
    ErreurPlateau err;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_plateau>\n", argv[0]);
        exit(1);
    }

    err = lire_fichier_plateau(argv[1], &P);
    if (err != OK) erreur_plateau(err);
    afficher_plateau(P);

    FNC* fnc = modeliser_jeu(P);
    fnc = sat_vers_3sat(fnc);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "3sat.dimacs");

#ifndef USING_CUSTOM_SOLVER
    // SOLVER
    solver* s = solver_new();
    lbool   st;
    FILE *  f_dimacs;

    f_dimacs = fopen("3sat.dimacs", "rb");
    if (f_dimacs == NULL) {
        fprintf(stderr, "ERROR! Could not open file: 3sat.dimacs\n");
        exit(1);
    }
    st = parse_DIMACS(f_dimacs, s);
    fclose(f_dimacs);

    s->verbosity = 0;
    st = solver_solve(s,0,0);

    // print the sat assignment
    if ( st == l_True )
    {
        int k;
        /* printf( "\nSatisfying solution: " );
        for ( k = 0; k < s->model.size; k++ )
            printf( "x%d=%d ", k, s->model.ptr[k] == l_True );
        printf( "\n" ); */

        // Remplissage du plateau avec les solutions
        for (k = 0; k < s->model.size; k++) {
            if (s->model.ptr[k] == l_True) { // cette variable est vraie
                VarLogique dec = decodage_id(*fnc, k);
     		    //printf("(%d, %d, %d)\n", dec.val, dec.x, dec.y);
                Case c;
                c.coor = creer_coor(dec.x, dec.y);
                c.type = TypeNombre;
                c.val.nombre = dec.val;
                modifier_case(&P, dec.x, dec.y, c);
            }
        }

        printf("\n> SOLUTION\n");
        afficher_plateau(P);
    } else {
        printf("Pas de solution, ce jeu est insatisfaisable.\n");
    }

    solver_delete(s);
#endif

#ifdef USING_CUSTOM_SOLVER
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
