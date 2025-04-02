#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../logique.h"

void erreur() {
    fprintf(stderr, "\033[41mErreur lors du test\033[0m\n");
    exit(1);
}

int main() {
    VarLogique v1, v2, v3;
    Clause cl1, cl2, cl3;
    FNC fnc;

    fnc = initialiser_FNC();
    printf("FNC Vide: \n");
    printf("Taille: %d\n", fnc.taille);
    if (fnc.taille != 0) erreur();
    afficher_FNC(fnc);

    cl1 = initialiser_clause();
    printf("\nClause vide:\n");
    printf("Taille: %d\n", cl1.taille);
    if (cl1.taille != 0) erreur();
    afficher_clause(cl1);


    printf("\nClause a un élément:\n");
    ajouter_variable_a_clause(&cl1, creer_var_logique(1, 2, 3, false));
    printf("Taille: %d\n", cl1.taille);
    afficher_clause(cl1);
    if (cl1.taille != 1 || cl1.variables[0].x != 1) erreur();
    printf("\n");


    return 0;
}
