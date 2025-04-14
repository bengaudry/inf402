#ifndef _SAT_SOLVER_H_
#define _SAT_SOLVER_H_

#define SS_MAX_VAR 50
#define SS_MAX_CLAUSES 1500

typedef int SS_VAR;

typedef struct {
    SS_VAR variables[SS_MAX_VAR];
    unsigned int taille;
} SS_CLAUSE;

typedef struct {
    SS_CLAUSE clauses[SS_MAX_CLAUSES];
    unsigned int taille;
    unsigned int nb_var;
    unsigned int nb_clauses;
} SS_FNC;

SS_FNC lire_fichier_dimacs(char *fichier_dimacs);
void afficher_fnc(SS_FNC fnc);

#endif
