#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "sat_solver.h"

SS_FNC lire_fichier_dimacs(char *fichier_dimacs) {
    SS_FNC fnc;
    FILE* f;
    char c;

    f = fopen(fichier_dimacs, "r");
    if (f == NULL) {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", fichier_dimacs);
        exit(1);
    }

    // Lecture des en-têtes
    if (fscanf(f, "p cnf %d %d\n", &fnc.nb_var, &fnc.nb_clauses) != 2) {
        fprintf(stderr, "Erreur lors de la lecture de l'en-tête du fichier.\n");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < fnc.nb_clauses; i++) {
        SS_CLAUSE cl;
        bool estneg = false;
        int val;

        cl.taille = 0;
        int lit;

        while (fscanf(f, "%d", &lit) == 1 && lit != 0) {
            if (cl.taille >= SS_MAX_VAR) {
                fprintf(stderr, "Clause trop longue.\n");
                exit(1);
            }
            cl.variables[cl.taille++] = lit;
        }

        if (lit != 0) {
            fprintf(stderr, "Clause mal terminée (attendu: 0).\n");
            exit(1);
        }
        fnc.clauses[i] = cl;
    }

    fclose(f);
    return fnc;
}

void afficher_fnc(SS_FNC fnc) {
    SS_CLAUSE cl;

    printf("p cnf %d %d\n", fnc.nb_var, fnc.nb_clauses);
    for (int i = 0; i < fnc.nb_clauses; i++) {
        cl = fnc.clauses[i];
        for (int j = 0; j < cl.taille; j++) {
            printf("%d ", cl.variables[j]);
        }
        printf("0\n");
    }
}
