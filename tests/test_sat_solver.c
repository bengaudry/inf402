#include <stdlib.h>
#include <stdio.h>
#include "../sat_solver.h"

int main (int argc, char **argv) {
    SS_FNC fnc;

    if (argc != 2) {
        fprintf(stderr, "Utilisation: %s <chemin_fichier_dimacs>\n", argv[0]);
        exit(1);
    }

    fnc = lire_fichier_dimacs(argv[1]);
    afficher_fnc(fnc);

    return 0;
}
