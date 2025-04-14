#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "sat_solver.h"

/* Lit un fichier dimacs (format 3-SAT) et renvoie un type FNC */
SS_FNC lire_fichier_dimacs(char *fichier_dimacs)
{
    SS_FNC fnc;
    FILE *f;
    char c;

    f = fopen(fichier_dimacs, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", fichier_dimacs);
        exit(1);
    }

    // Lecture des en-têtes
    if (fscanf(f, "p cnf %d %d\n", &fnc.nb_var, &fnc.nb_clauses) != 2)
    {
        fprintf(stderr, "Erreur lors de la lecture de l'en-tête du fichier.\n");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < fnc.nb_clauses; i++)
    {
        SS_CLAUSE cl;
        fscanf(f, "%d %d %d 0\n", &cl.v0, &cl.v1, &cl.v2);
        fnc.clauses[i] = cl;
    }

    fclose(f);
    return fnc;
}

void afficher_fnc(SS_FNC fnc)
{
    SS_CLAUSE cl;

    printf("p cnf %d %d\n", fnc.nb_var, fnc.nb_clauses);
    for (int i = 0; i < fnc.nb_clauses; i++)
    {
        cl = fnc.clauses[i];
        printf("%d %d %d 0\n", cl.v0, cl.v1, cl.v2);
    }
}

/* Résoud un problème de type 3-SAT et renvoie vrai si le problème admet une solution
 * Si c'est le cas, le tableau contenant la solution est passé dans le pointeur rep
 */
bool solve_3sat(SS_FNC fnc, SS_REPONSE* rep) {
    return false;
}
