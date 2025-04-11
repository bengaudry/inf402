#include <stdlib.h>
#include <stdio.h>

#include "dimacs.h"

int encodage_id(int dim, int maxVal, VarLogique var) {
    int id = ((var.x - 1)*dim + (var.y - 1))*maxVal + var.val;

    //si la variable est une négation
    if (var.isneg == true) {
        return -id;
    }
    //sinon
    return id;
}

void decodage_id(int dim, int maxVal, int id, int* x, int* y, int* val) {
    if (id < 0) id = -id;
    int case_id = (id - 1) / maxVal; // index de la case dans la grille linéaire
    *val = ((id - 1) % maxVal) + 1;  // position dans le bloc, donc la valeur

    *x = (case_id / dim) + 1;         // ligne (on remet de 0 à 1-indexé)
    *y = (case_id % dim) + 1;         // colonne (idem)
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