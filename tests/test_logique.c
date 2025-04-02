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
    FNC *fnc;

    fnc = initialiser_FNC();
    cl1 = initialiser_clause();
    cl2 = initialiser_clause();
    cl3 = initialiser_clause();

    printf("> FNC Vide: \n");
    printf("Taille: %d\n", fnc->taille);
    if (fnc->taille != 0) erreur();
    afficher_FNC(*fnc);

    printf("\n> Clause vide:\n");
    printf("Taille: %d\n", cl1.taille);
    if (cl1.taille != 0) erreur();
    afficher_clause(cl1);

    printf("\n> Clause a un élément:\n");
    ajouter_variable_a_clause(&cl1, creer_var_logique(1, 2, 3, false));
    printf("Taille: %d\n", cl1.taille);
    afficher_clause(cl1);
    if (cl1.taille != 1 || cl1.variables[0].val != 1) erreur();

    printf("\n\n> Clause a plusieurs éléments:\n");
    ajouter_variable_a_clause(&cl1, creer_var_logique(4, 1, 3, false));
    printf("Taille: %d\n", cl1.taille);
    afficher_clause(cl1);
    if (cl1.taille != 2 || cl1.variables[0].val != 1 || cl1.variables[1].val != 4)
        erreur();

    ajouter_variable_a_clause(&cl2, creer_var_logique(2, 1, 3, true));
    ajouter_variable_a_clause(&cl2, creer_var_logique(1, 3, 2, false));
    ajouter_variable_a_clause(&cl2, creer_var_logique(0, 2, 1, true));
    ajouter_variable_a_clause(&cl3, creer_var_logique(1, 1, 2, true));

    ajouter_clause_a_fnc(fnc, cl1);
    ajouter_clause_a_fnc(fnc, cl2);
    ajouter_clause_a_fnc(fnc, cl3);

    printf("\n\n> FNC Complète: \n");
    printf("Taille: %d\n", fnc->taille);
    afficher_FNC(*fnc);

    return 0;
}
