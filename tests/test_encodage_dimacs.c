#include <stdlib.h>
#include <stdio.h>

#include "../dimacs.h"

int main(int argc, char **argv) {

    /*----- TESTS CALCUL ID -----*/
    int dim = 5;
    int maxVal = 4;
    VarLogique var = creer_var_logique(1, 1, 2, false);

    //affichage variable logique iinitale
    printf("Variable logique initiale: ");
    afficher_var_logique(var);
    printf("\n");

    //calcul de l'id unique correspondant à la variable

    int id = encodage_id(dim, maxVal, var);
    printf("ID calculé: %d\n", id);

    //calcul de la variable à partir de l'id
    decodage_id(dim, maxVal, id, &var.x, &var.y, &var.val);
    printf("Variable logique depuis l'id: ");
    afficher_var_logique(var);
    printf("\n");
    
    return 0;
}

