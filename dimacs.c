#include <stdlib.h>
#include <stdio.h>

#include "dimacs.h"

int encodage_id(int dim, int maxVal, VarLogique var) {
    int id = (var.val - 1)*dim*dim + (var.x - 1)*dim + var.y;

    //si la variable est une négation
    if (var.isneg == true) {
        return -id;
    }
    //sinon
    return id;
}

void decodage_id(int dim, int maxVal, int id, int* x, int* y, int* val) {
    if (id < 0) id = -id;
    id -= 1;
    *x = (id / (dim * dim)) + 1;
    *y = ((id % (dim * dim)) / dim) + 1;
    *val = (id % dim) + 1;
}

void sortie_dimacs(FNC fnc, int dim, int maxVal, char *fichier_sortie){
    FILE *f;
    f = fopen(fichier_sortie, "w");
    CellFNC *cel = fnc.first;
    VarLogique var;
    int nb_var = nombre_var(fnc);
    int id;

    //en-tete du fichier
    fprintf(f, "p cnf %d %d\n", nb_var, fnc.taille);

    //corps du fichier
    while (cel != NULL) {
        //ecriture d'une clause
        for (int i=0; i<cel->clause.taille; i++){
            var = cel->clause.variables[i];
            id = encodage_id(dim, maxVal, var);

            //ecriture de l'id
            fprintf(f, "%d ", id);
        }
        //fin de ligne
        fprintf(f, "0\n");
        cel = cel->suiv;
    }
    fclose(f);
}