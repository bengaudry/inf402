#ifndef _FNC_H_
#define _FNC_H_

#define MAX_VAR 50

#include <stdbool.h>

// Variable logique représentant la variable X(x, y, k)
// ou k est la valeur de la case (x, y)
typedef struct {
    int val;
    int x;
    int y;
    bool isneg;
} VarLogique;

// Clause : ensemble de disjonctions de variables
typedef struct {
    VarLogique variables[MAX_VAR];
    unsigned int taille;
} Clause;

typedef struct _cell_fnc {
    Clause clause;
    struct _cell_fnc *suiv;
} CellFNC;

// FNC: Ensemble de clauses
typedef struct {
    CellFNC* first;
    CellFNC* last;
    unsigned int taille;
} FNC;

/* ====== FONCTIONS ======*/

/* Initialise une variable logique correspodant à X = (x, y, val) ou ¬X si isneg = true */
VarLogique creer_var_logique(int val, int x, int y, bool isneg);

/* Initialise une clause vide */
Clause initialiser_clause();
/* Ajoute une variable à une clause (liste de variables) */
void ajouter_variable_a_clause(Clause* cl, VarLogique var);

/* Initialise une forme normale conjonctive vide (liste de clauses) */
FNC* initialiser_FNC();
/* Ajoute une clause a une forme normale conjonctive */
void ajouter_clause_a_fnc(FNC* fnc, Clause cl);

void afficher_var_logique(VarLogique var);
void afficher_clause(Clause cl);
void afficher_FNC(FNC fnc);

#endif
