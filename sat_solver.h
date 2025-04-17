#ifndef _SAT_SOLVER_H_
#define _SAT_SOLVER_H_

#define SS_MAX_CLAUSES 1500

typedef struct {
    int v0;
    int v1;
    int v2;
} SS_CLAUSE;

typedef struct {
    SS_CLAUSE clauses[SS_MAX_CLAUSES];
    unsigned int nb_var;
    unsigned int nb_clauses;
} SS_FNC;

typedef struct {
    int valeurs[SS_MAX_CLAUSES*3];
    unsigned int taille;
} SS_REPONSE;

SS_FNC lire_fichier_dimacs(char *fichier_dimacs);

void afficher_fnc(SS_FNC fnc);

bool eval_clause(SS_CLAUSE clause, bool* assignation);

/* Résoud un problème de type 3-SAT et renvoie vrai si le problème admet une solution
 * Si c'est le cas, le tableau contenant la solution est passé dans le pointeur rep
 */
bool solve_3sat(SS_FNC, SS_REPONSE* rep);


SS_FNC dpll_reduction(SS_FNC fnc);
SS_FNC dpll_elim_litteraux_isoles(SS_FNC fnc);
int dpll_assignation_variable(SS_FNC fnc, SS_FNC* assign, bool var_vraie);
bool dpll_solver(SS_FNC fnc, SS_REPONSE *rep);

#endif
