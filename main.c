#include <stdlib.h>
#include <stdio.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"
#include "minisat_src/minisat.h"
#include "minisat_src/solver.h"

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
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "sat.dimacs");
    afficher_FNC(*fnc);


    // SOLVER
    solver* s = solver_new();
    lbool   st;
    FILE *  in;
    int     clk = clock();

    in = fopen("sat.dimacs", "rb");
    if (in == NULL)
        fprintf(stderr, "ERROR! Could not open file: sat.dimacs\n"),
        exit(1);
    st = parse_DIMACS(in, s);
    fclose(in);

    if (st == l_False){
        solver_delete(s);
        printf("Trivial problem\nUNSATISFIABLE\n");
        exit(20);
    }

    s->verbosity =0;
    //    slv = s;
    //    signal(SIGINT,SIGINT_handler);
    st = solver_solve(s,0,0);
    //printStats(&s->stats, clock() - clk);
    //printf("\n");
    //printf(st == l_True ? "SATISFIABLE\n" : "UNSATISFIABLE\n");

    // print the sat assignment
    if ( st == l_True )
    {
        int k;
        printf( "\nSatisfying solution: " );
        for ( k = 0; k < s->model.size; k++ )
            printf( "x%d=%d ", k, s->model.ptr[k] == l_True );
        printf( "\n" );

        Plateau P_Solution;
        P_Solution.dim = P.dim;
        P_Solution.nb_salles = P.nb_salles;
        *P_Solution.salles = *P.salles;
        P_Solution.val_max = P.val_max;

        // Duplication du plateau
        for (int x = 1; x <= P.dim; x++) {
            for (int y = 1; y <= P.dim; y++) {
                modifier_case(&P_Solution, x, y, case_plateau(P, x, y));
            }
        }

        // Remplissage du plateau avec les solutions
        for (k = 0; k < s->model.size; k++) {
            if (s->model.ptr[k] == l_True) { // cette variable est vraie
                VarLogique dec = decodage_id(*fnc, k);
                printf("(%d, %d, %d)\n", dec.val, dec.x, dec.y);
                Case c;
                c.coor = creer_coor(dec.x, dec.y);
                c.type = TypeNombre;
                c.val.nombre = dec.val;
                modifier_case(&P_Solution, dec.x, dec.y, c);
            }
        }

        printf("\n> SOLUTION\n");
        afficher_plateau(P_Solution);
    } else {
        printf("Pas de solution, ce jeu est insatisfaisable.\n");
    }

    solver_delete(s);
    return 0;

    return 0;
}
