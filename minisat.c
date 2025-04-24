#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "logique.h"
#include "dimacs.h"
#include "plateau.h"
#include "modelisation.h"

bool generer_solution(Plateau P, Plateau* P_solution) {
    FNC* fnc;

    *P_solution = P; // Copie de P dans P_solution
    fnc = modeliser_jeu(P);
    sortie_dimacs(*fnc, dimension_plateau(P), val_max_plateau(P), "sat.dimacs");

    system("./minisat/MiniSat_v1.14_linux sat.dimacs .tmp > /dev/null");
    FILE *rep = fopen(".tmp", "r");
    if (!rep) {
        fprintf(stderr, "Pas de fichier réponse généré, erreur\n");
        exit(1);
    }

    char line[100];
    fgets(line, 100, rep);
    if (strcmp(line, "SAT\n") == 0) {
        int k;
        while (fscanf(rep, "%d ", &k)) {
            if (k == 0) break;
            if (k > 0) {
                VarLogique dec = decodage_id(*fnc, k-1);
                Case c;
                c.coor = creer_coor(dec.x, dec.y);
                c.type = TypeNombre;
                c.val.nombre = dec.val;
                modifier_case(P_solution, dec.x, dec.y, c);
            }
        }
        fclose(rep);
        return true;
    }
    
    fclose(rep);
    return false;
}
