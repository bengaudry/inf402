#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"
#include "commun.h"

#include "minisat_src/minisat.h"
#include "minisat_src/solver.h"

int nombre_cases_pleines(Plateau P){
    int nb_cases_pleines = 0;
    Case c;

    for (int x=1; x<=dimension_plateau(P); x++) {
        for (int y=1; y<=dimension_plateau(P); y++) {
            c = case_plateau(P, x, y);
            if (c.type != TypeVide) {
                nb_cases_pleines++;
            }
        }
    }
    return nb_cases_pleines;
}

bool fleches_egales(Fleche f1, Fleche f2) {
    return (coor_egales(f1.case_pointee, f2.case_pointee) && f1.or == f2.or);
}

bool cases_egales(Case c1, Case c2) {
    if (c1.type == TypeNombre) {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type && c1.val.nombre == c2.val.nombre);
    }
    else if (c1.type == TypeVide) {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type);
    }
    else {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type && fleches_egales(c1.val.fleche, c2.val.fleche));
    }
}

bool plateaux_egaux(Plateau P1, Plateau P2) {
    int dim = dimension_plateau(P1);

    for (int x=1; x<=dim; x++) {
        for (int y=1; y<=dim; y++) {
            if (cases_egales(case_plateau(P1, x, y), case_plateau(P2, x, y)) == false) {
                return false;
            }
        }
    }
    return true;
}

//arg1: chemin_fichier_plateau
int main(int argc, char **argv) {

    /*----- LECTURE PLATEAU -----*/
    Plateau P;
    Plateau P_joueur;
    ErreurPlateau err;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_plateau>\n", argv[0]);
        exit(1);
    }

    err = lire_fichier_plateau(argv[1], &P);
    if (err != OK) erreur_plateau(err);
    P_joueur = P;
    //afficher_plateau(P);

    FNC* fnc = modeliser_jeu(P);

    //afficher_FNC(*fnc);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "jeu.dimacs");

    /*----- SOLVER -----*/
    solver* s = solver_new();
    lbool   st;
    FILE *  f_dimacs;

    f_dimacs = fopen("jeu.dimacs", "rb");
    if (f_dimacs == NULL) {
        fprintf(stderr, "ERROR! Could not open file: jeu.dimacs\n");
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
        //printf( "\nSatisfying solution: " );
        for ( k = 0; k < s->model.size; k++ )
            //printf( "x%d=%d ", k, s->model.ptr[k] == l_True );
        //printf( "\n" );

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

        //printf("\n> SOLUTION\n");
        //afficher_plateau(P);
    } else {
        printf("Pas de solution, ce jeu est insatisfaisable.\n");
        return 1;
    }
    solver_delete(s);    

    /*----- JOUER PARTIE -----*/
    int x,y;
    int nb_cases_total = dimension_plateau(P)*dimension_plateau(P);
    int nb_cases_plns = nombre_cases_pleines(P_joueur);
    Case c;
    c.type = TypeNombre;
    int choix;

    afficher_plateau(P_joueur);
    //boucle de jeu
    while (nb_cases_plns < nb_cases_total) {
        //afficher_plateau(P_joueur);
        printf("\n(1) pour demander un indice\n");
        printf("(2) pour remplir une case\n");
        scanf("%d", &choix);

        while (choix != 1 && choix != 2) {
            printf("\n(1) pour demander un indice\n");
            printf("(2) pour remplir une case\n");
            scanf("%d", &choix);
        }

        //donner un indice
        if (choix == 1) {
            printf("\nEntrez une case à dévoiler: x y\n");
            scanf("%d %d", &x, &y);
    
            //verifier les bonnes coordonnees
            while (case_dans_plateau(P_joueur, creer_coor(x, y)) == false) {
                printf("\nCoordonnees incorrectes : 1 <= x,y <= %d\n", dimension_plateau(P_joueur));
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &x, &y);
            }

            //verifier que la case contienne un nombre
            while (case_plateau(P, x, y).type != TypeNombre) {
                printf("\nCette case contient une flèche\n");
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &x, &y);
            }
            //si la case etait vide on incremente
            if (case_plateau(P_joueur, x, y).type == TypeVide) nb_cases_plns++;
            //modification de la case indice choisie
            modifier_case(&P_joueur, x, y, case_plateau(P, x, y));
        }

        //remplir une case (choix == 2)
        else {
            printf("\nEntrez une case: x y valeur:\n");
            scanf("%d %d %d", &c.coor.x, &c.coor.y, &c.val.nombre);
            
            //verifier les bonnes coordonnees
            while (case_dans_plateau(P_joueur, creer_coor(c.coor.x, c.coor.y)) == false) {
                printf("\nCoordonnees incorrectes : 1 <= x,y <= %d\n", dimension_plateau(P_joueur));
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &c.coor.x, &c.coor.y);
            }

            //verifier que la case peut recevoir un nombre : != typeFleche
            while (case_plateau(P_joueur, c.coor.x, c.coor.y).type == TypeFleche) {
                printf("\nCette case contient une flèche\n");
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &c.coor.x, &c.coor.y);
            }            

            //on incrémente si la case etait vide
            if (case_plateau(P_joueur, c.coor.x, c.coor.y).type == TypeVide) nb_cases_plns++;
            //modification de la case
            modifier_case(&P_joueur, c.coor.x, c.coor.y, c);
        }
        printf("\n");
        afficher_plateau(P_joueur);
    }

    if (plateaux_egaux(P_joueur, P)) {
        printf("\n\nBRAVO, niveau réussi\n");
        return 0;
    }

    printf("\n\nNiveau échoué... voici le plateau réponse\n");
    afficher_plateau(P);
    return 0;
}