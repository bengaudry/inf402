#include <stdlib.h>
#include <stdio.h>

#include "dimacs.h"
#include "logique.h"

int encodage_id(FNC fnc, VarLogique var) {
    for (int i = 0; i < fnc.nb_variables; i++) {
        if (var_logiques_equivalentes(var, fnc.liste_variables[i])) return var.isneg ? -(i+1) : i+1;
    } 
    return -1;
}

VarLogique decodage_id(FNC fnc, int id) {
    return fnc.liste_variables[id];
}

void sortie_dimacs(FNC fnc, int dim, int maxVal, char *fichier_sortie){
    FILE *f;
    f = fopen(fichier_sortie, "w");
    CellFNC *cel = fnc.first;
    VarLogique var;
    int nb_var = nombre_var(fnc);
    int id;

    //en-tete du fichier
    fprintf(f, "p cnf %d %d\n", fnc.nb_variables, fnc.taille);

    //corps du fichier
    while (cel != NULL) {
        //ecriture d'une clause
        for (int i=0; i<cel->clause.taille; i++){
            var = cel->clause.variables[i];
            //id = encodage_id(dim, maxVal, var);
            id = encodage_id(fnc, var);

            //ecriture de l'id
            fprintf(f, "%d ", id);
        }
        //fin de ligne
        fprintf(f, "0\n");
        cel = cel->suiv;
    }
    fclose(f);
}
