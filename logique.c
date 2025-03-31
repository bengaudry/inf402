#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "logique.h"

VarLogique creer_var_logique(int val, int x, int y, bool isneg) {
    VarLogique var;
    var.val = val;
    var.x = x;
    var.y = y;
    var.isneg = isneg;
    return var;
}

Clause initialiser_clause() {
    Clause cl;
    cl.taille = 0;
    return cl;
}

void ajouter_variable_a_clause(Clause* cl, VarLogique var) {
    cl->variables[cl->taille++] = var;
}

FNC initialiser_FNC() {
    FNC fnc;
    fnc.first = NULL;
    fnc.last = NULL;
    fnc.taille = 0;
    return fnc;
}

void ajouter_clause_a_fnc(FNC* fnc, Clause cl) {
    CellFNC *cell;

    if (fnc == NULL) {
        fprintf(stderr, "Erreur: FNC non initialisée\n");
        exit(1);
    }

    cell = malloc(sizeof(CellFNC));
    cell->clause = cl;

    if (fnc->taille == 0) {
        cell->suiv = NULL;
        fnc->first = cell;
        fnc->last = cell;
        fnc->taille = 1;
        return;
    }

    cell->suiv = NULL;
    fnc->last = cell;
    fnc->taille++;
}

void afficher_var_logique(VarLogique var) {
    if (var.isneg) printf("¬");
    printf("(%d, %d, %d)", var.x, var.y, var.val);
}

void afficher_clause(Clause cl) {
    unsigned int i;
    unsigned int taille;

    taille = cl.taille;
    if (taille == 0) return;

    for (i = 0; i < taille-1; i++) {
        afficher_var_logique(cl.variables[i]);
        printf("∨");
    }

    if (i < taille) afficher_var_logique(cl.variables[taille]);
}

void afficher_FNC(FNC fnc) {
    CellFNC* cell;

    cell = fnc.first;
    while (cell != NULL) {
        afficher_clause(cell->clause);
        printf("\n");
        cell = cell->suiv;
    }
}
