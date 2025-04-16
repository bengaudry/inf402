#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "../sat_solver.h"

int main (int argc, char **argv) {
    SS_FNC fnc;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_dimacs>\n", argv[0]);
        exit(1);
    }

    fnc = lire_fichier_dimacs(argv[1]);
    afficher_fnc(fnc);

    SS_REPONSE rep;
    rep.taille = 0;
    bool succes = solve_3sat(fnc, &rep);

    if (!succes) {
        printf("INSAT\n");
        return 1;
    }

    for (int i = 0;  i <= rep.taille; i++) {
        printf("%d ", rep.valeurs[i]);
    }
    printf("\n");

    return 0;
}
