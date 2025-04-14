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
    unsigned int taille;
    unsigned int nb_var;
    unsigned int nb_clauses;
} SS_FNC;

typedef struct {
    int valeurs[SS_MAX_CLAUSES*3];
    unsigned int taille;
} SS_REPONSE;

SS_FNC lire_fichier_dimacs(char *fichier_dimacs);
void afficher_fnc(SS_FNC fnc);

#endif
