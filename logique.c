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
    if (cl->taille >= MAX_VAR) {
        fprintf(stderr, "Erreur: Clause pleine (MAX_VAR = %d)\n", MAX_VAR);
        exit(1);
    }
    cl->variables[cl->taille] = var;
    cl->taille++;
}

FNC* initialiser_FNC() {
    FNC *fnc;
    fnc = malloc(sizeof(FNC));
    fnc->first = NULL;
    fnc->last = NULL;
    fnc->taille = 0;
    fnc->nb_variables = 0;
    return fnc;
}

/* Renvoie vrai si deux variables logiques sont égales à la négation près */
bool var_logiques_equivalentes(VarLogique A, VarLogique B) {
    return (A.val == B.val && A.x == B.x && A.y == B.y);
}

/* Renvoie vrai si une variable est déjà présente dans une clause de la fnc */
bool variable_existe_dans_fnc(FNC fnc, VarLogique var) {
    CellFNC* cel;
    Clause cl;

    cel = fnc.first;
    while (cel != NULL) { // pour chaque clause
        cl = cel->clause;
        for (int i = 0; i < cl.taille; i++) { // pour chaque variable de la clause
            VarLogique v2 = cl.variables[i];
            if (var_logiques_equivalentes(v2, var)) return true;
        }
        cel = cel->suiv;
    }
    return false;
}

void ajouter_clause_a_fnc(FNC* fnc, Clause cl) {
    CellFNC *cell;

    if (fnc == NULL) {
        fprintf(stderr, "Erreur: FNC non initialisée\n");
        exit(1);
    }

    cell = malloc(sizeof(CellFNC));
    cell->clause = cl;
    cell->suiv = NULL;

    for (int i = 0; i < cl.taille; i++) {
        VarLogique var = cl.variables[i];
        if (!variable_existe_dans_fnc(*fnc, var)) {
            // Une nouvelle var est introduite
            fnc->liste_variables[fnc->nb_variables++] = creer_var_logique(var.val, var.x, var.y, false);
        }
    }

    if (fnc->taille == 0) {
        fnc->first = cell;
        fnc->last = cell;
        fnc->taille = 1;
        return;
    }

    fnc->last->suiv = cell;
    fnc->last = cell;
    fnc->taille++;
}

/* Transforme un problème de type SAT en type 3-SAT */
FNC* sat_vers_3sat(FNC* fnc_sat) {
    FNC* fnc_3sat;
    CellFNC* cel;
    Clause cl_sat, cl_3sat;

    fnc_3sat = initialiser_FNC();

    // permet de compter le nombre de variables introduites pour les clauses de plus de 
    // 3 variables pour éviter de confondre avec des variables du plateau, ou des variables
    // déjà introduites lors d'une transformation précédente
    int nb_var_introduites = 0; 

    cel = fnc_sat->first;
    while (cel != NULL) {
        cl_sat = cel->clause;

        // {x} <=> {x+x+x}
        if (cl_sat.taille == 1) {
            cl_3sat = initialiser_clause();
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
        }

        // {x+y} <=> {x+x+y}
        if (cl_sat.taille == 2) {
            cl_3sat = initialiser_clause();
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[1]);
        }

        if (cl_sat.taille == 3) {
            cl_3sat = cl_sat;
        }

        if (cl_sat.taille > 3) {
            // (x1+x2+...+xn) <=> (x1+x2+z1)*(¬z1+x3+z2)*...*(¬zn-4+xn-2+zn-3)*(¬zn-3+xn-1+xn)
            // avec z1, ..., zn-3 des variables introduites

            // Clause 1 : (x1+x2+z1)
            cl_3sat = initialiser_clause();

            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[0]);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[1]);
            nb_var_introduites += 1;
            VarLogique z1 = creer_var_logique(-nb_var_introduites, -nb_var_introduites, -nb_var_introduites, false); // (z1)
            ajouter_variable_a_clause(&cl_3sat, z1);

            ajouter_clause_a_fnc(fnc_3sat, cl_3sat);

            // Clauses du milieu : (¬zk-1+xk+zk)
            for (int k = 2; k <= cl_3sat.taille; k++) {
                cl_3sat = initialiser_clause();

                VarLogique zp = creer_var_logique(-nb_var_introduites, -nb_var_introduites, -nb_var_introduites, true); //(¬zk-1)
                ajouter_variable_a_clause(&cl_3sat, zp); 
                ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[k]); // xk
                nb_var_introduites += 1;
                VarLogique zd = creer_var_logique(-nb_var_introduites, -nb_var_introduites, -nb_var_introduites, false); //(zk)
                ajouter_variable_a_clause(&cl_3sat, zd);

                ajouter_clause_a_fnc(fnc_3sat, cl_3sat);
            }

            cl_3sat = initialiser_clause();

            VarLogique zn = creer_var_logique(-nb_var_introduites, -nb_var_introduites, -nb_var_introduites, true); // (zn-3)
            ajouter_variable_a_clause(&cl_3sat, zn);
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[cl_sat.taille-2]); // (xn-1)
            ajouter_variable_a_clause(&cl_3sat, cl_sat.variables[cl_sat.taille-1]); // (xn)

        }
        ajouter_clause_a_fnc(fnc_3sat, cl_3sat);
        cel = cel->suiv;
    }

    return fnc_3sat;
}


/* ====== FONCTIONS D'AFFICHAGE ======*/
void afficher_var_logique(VarLogique var) {
    if (var.isneg) printf("¬");
    printf("(%d, %d, %d)", var.val, var.x, var.y);
}

void afficher_clause(Clause cl) {
    unsigned int i;
    unsigned int taille;

    taille = cl.taille;
    if (taille == 0) return;

    for (i = 0; i < taille-1; i++) {
        afficher_var_logique(cl.variables[i]);
        printf(" ∨ ");
    }

    if (i < taille) afficher_var_logique(cl.variables[taille-1]);
}

void afficher_FNC(FNC fnc) {
    CellFNC* cell;

    cell = fnc.first;
    while (cell != NULL && cell->suiv != NULL) {
        afficher_clause(cell->clause);
        printf(",\n");
        cell = cell->suiv;
    }

    if (cell != NULL) {
        afficher_clause(cell->clause);
        printf("\n");
    }
}

int nombre_var(FNC fnc) {
    CellFNC *cel = fnc.first;
    int nb = 0;

    while (cel != NULL){
        nb += cel->clause.taille;
        cel = cel->suiv;
    }
    return nb;
}
