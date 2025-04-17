#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>
#include "sat_solver.h"

/* Lit un fichier dimacs (format 3-SAT) et renvoie un type FNC */
SS_FNC lire_fichier_dimacs(char *fichier_dimacs)
{
    SS_FNC fnc;
    FILE *f;
    char c;

    f = fopen(fichier_dimacs, "r");
    if (f == NULL)
    {
        fprintf(stderr, "Erreur lors de l'ouverture du fichier : %s\n", fichier_dimacs);
        exit(1);
    }

    // Lecture des en-têtes
    if (fscanf(f, "p cnf %d %d\n", &fnc.nb_var, &fnc.nb_clauses) != 2)
    {
        fprintf(stderr, "Erreur lors de la lecture de l'en-tête du fichier.\n");
        fclose(f);
        exit(1);
    }

    for (int i = 0; i < fnc.nb_clauses; i++)
    {
        SS_CLAUSE cl;
        fscanf(f, "%d %d %d 0\n", &cl.v0, &cl.v1, &cl.v2);
        fnc.clauses[i] = cl;
    }

    fclose(f);
    return fnc;
}

void afficher_fnc(SS_FNC fnc)
{
    SS_CLAUSE cl;

    printf("p cnf %d %d\n", fnc.nb_var, fnc.nb_clauses);
    for (int i = 0; i < fnc.nb_clauses; i++)
    {
        cl = fnc.clauses[i];
        printf("%d %d %d 0\n", cl.v0, cl.v1, cl.v2);
    }
}

bool eval_clause(SS_CLAUSE clause, bool* assignation){
    int literals[3] = {clause.v0, clause.v1, clause.v2};
    int i;
    for (i = 0; i < 3; i++) { //on parcourt toutes les clauses pour verifier qu'elles sont toutes true
        int var = abs(literals[i])-1;      //(false car on prends l'inverse en retour de fonction)
        bool expected = literals[i]>0;
        if (assignation[var] == expected) return true;
    }
    return false;
}

/* Résoud un problème de type 3-SAT et renvoie vrai si le problème admet une solution
 * Si c'est le cas, le tableau contenant la solution est passé dans le pointeur rep
 */
bool solve_3sat(SS_FNC fnc, SS_REPONSE* rep){
    unsigned int n = fnc.nb_var;
    unsigned int nb_assign = 1<<n; //2^n possibilités
    bool assignation[n];
    unsigned int i, j, k;

    for (i = 0; i < nb_assign; i++) {
        //assignation booléenne à partir de i
        for (j = 0; j < n; j++) {
            assignation[j]=(i &(1<<j))!=0;
        }

        //vérifier toutes les clauses
        bool satisfaite=true;
        for (k = 0; k < fnc.nb_clauses; k++) {
            if (!eval_clause(fnc.clauses[k], assignation)) {
                //si une des clauses n'est pas satisfaite, on renvoie false
                satisfaite=false;
                break;
            }
        }

        if (satisfaite) {
            //remplir rep avec l'assignation trouvée
            j = 0;
            rep->taille=n;
            for (j = 0; j < n; j++) {
                if (assignation[j]) {
                    rep->valeurs[j]=j+1;  //j+1 assignée à true donc positif
                }
                else {
                    rep->valeurs[j]= -(j+1);  //sinon est assignée à false donc négatif
                }
                
            }
            return true;
        }
    }

    //aucune assignation satisfaisante
    rep->taille=0;
    return false;
}




// DPLL //

inline int abs(int x) { return x < 0 ? -x : x; }

bool fnc_contient_clause(SS_FNC fnc, SS_CLAUSE cl1) {
    SS_CLAUSE cl2;

    for (int i = 0; i < fnc.nb_clauses; i++) {
        if (cl1.v0 != cl2.v0 || cl1.v1 != cl2.v1 || cl1.v2 != cl2.v2) {   
            return true;
        }
    }
    return false;
}

/** Étape de réduction du DPLL */
SS_FNC dpll_reduction(SS_FNC fnc) {
    SS_FNC red;
    unsigned int nb_clauses = 0;

    for (unsigned int i = 0; i < fnc.nb_clauses; i++) {
        bool inclue = false;
        for (unsigned int j = 0; j < fnc.nb_clauses; j++) {
            if (i != j) {
                SS_CLAUSE c1 = fnc.clauses[i];
                SS_CLAUSE c2 = fnc.clauses[j];
                if ((c1.v0 == c2.v0 || c1.v0 == c2.v1 || c1.v0 == c2.v2) &&
                    (c1.v1 == c2.v0 || c1.v1 == c2.v1 || c1.v1 == c2.v2) &&
                    (c1.v2 == c2.v0 || c1.v2 == c2.v1 || c1.v2 == c2.v2)) {
                    inclue = true;
                    break;
                }
            }
        }
        if (!inclue) {
            red.clauses[nb_clauses++] = fnc.clauses[i];
        }
    }
    red.nb_clauses = nb_clauses;
    red.nb_var = fnc.nb_var;
    return red;
}

SS_CLAUSE reorganiser_clause(SS_CLAUSE cl) {
    SS_CLAUSE clr;

    int nb_zeros = 0;
    if (cl.v0 == 0) nb_zeros++;
    if (cl.v1 == 0) nb_zeros++;
    if (cl.v2 == 0) nb_zeros++;

    if (nb_zeros == 0) return cl;

    clr.v0 = 0;
    clr.v1 = 0;
    clr.v2 = 0;

    if (nb_zeros == 3) return clr;

    if (nb_zeros == 1) {
        if (cl.v0 == 0) {
            clr.v0 = cl.v1;
            clr.v1 = cl.v2;
        } else if (cl.v1 == 0) {
            clr.v0 = cl.v0;
            clr.v1 = cl.v2;
        } return cl;
    }

    if (nb_zeros == 2) {
        if (cl.v0 == 0) {
            if (cl.v1 == 0) {
                clr.v0 = cl.v2;
            } else clr.v0 = cl.v1;
        } else return cl;
    }

    return clr;
}

SS_FNC dpll_elim_litteraux_isoles(SS_FNC fnc) {
    SS_CLAUSE cl, clp;
    SS_FNC filtree;

    filtree.nb_clauses = 0;
    filtree.nb_var = fnc.nb_var;

    for (unsigned int i = 0; i < fnc.nb_clauses; i++) {
        cl = fnc.clauses[i];
        if (cl.v0 == 0) continue; // Ne pas ajouter clause vide
        if (cl.v1 == 0 && cl.v2 == 0) {
            clp.v0 = abs(cl.v0);
            clp.v1 = 0;
            clp.v2 = 0;
            filtree.clauses[filtree.nb_clauses++] = clp;
        } else filtree.clauses[filtree.nb_clauses++] = cl;
    }
    return filtree;
}

int dpll_assignation_variable(SS_FNC fnc, SS_FNC* assign, bool var_vraie) {
    int var;
    SS_CLAUSE cl1, cl2;

    assign->nb_clauses = 0;
    assign->nb_var = fnc.nb_var;

    if (fnc.nb_clauses == 0) {
        *assign = fnc;
        return 0;
    };
    var = abs(fnc.clauses[0].v0);

    printf("\n\nAssign <%d:=1>\n", var);

    for (unsigned int i = 0; i < fnc.nb_clauses; i++) {
        cl1 = fnc.clauses[i];
        // x := 1 : (a, x, b) -> 1, (a, -x, b) -> (a, b, 0)
        // x := 0 : (a, -x, b) -> 1, (a, x, b) -> (a, b, 0)

        if (var_vraie) {
            if (cl1.v0 == var || cl1.v1 == var || cl1.v2 == var) {
                // La clause contient la variable, on ne l'ajoute pas
                continue;
            }

            // Suppression des variables neg
            cl2.v0 = cl1.v0 == -var ? 0 : cl1.v0;
            cl2.v1 = cl1.v1 == -var ? 0 : cl1.v1;
            cl2.v2 = cl1.v2 == -var ? 0 : cl1.v2;
            cl2 = reorganiser_clause(cl2);
        } else {
            if (cl1.v0 == -var || cl1.v1 == -var || cl1.v2 == -var) {
                // La clause contient la variable, on ne l'ajoute pas
                continue;
            }

            // Suppression des variables neg
            cl2.v0 = cl1.v0;
            cl2.v1 = cl1.v1;
            cl2.v2 = cl1.v2;
            if (cl1.v0 == var) cl2.v0 = 0;
            if (cl1.v1 == var) cl2.v1 = 0;
            if (cl1.v2 == var) cl2.v2 = 0;
            cl2 = reorganiser_clause(cl2);
        }
        if (cl2.v0 != 0 || cl2.v1 != 0 || cl2.v2 != 0) {
            assign->clauses[assign->nb_clauses++] = cl2;
        }
    }
    return var;
}

bool existe_clause_fausse(SS_FNC fnc) {
    SS_CLAUSE cl;

    for (int i = 0; i < fnc.nb_clauses; i++) {
        cl = fnc.clauses[i];
        if (cl.v0 != 0 && cl.v1 != 0 && cl.v0 == -cl.v1) return true;
        if (cl.v0 != 0 && cl.v2 != 0 && cl.v0 == -cl.v2) return true;
        if (cl.v1 != 0 && cl.v2 != 0 && cl.v1 == -cl.v2) return true;
    }
    return false;
}

bool dpll_solver(SS_FNC fnc, SS_REPONSE *rep) {
    SS_FNC fnc_v, fnc_f;
    unsigned int nb_cl_prec = fnc.nb_clauses;

    afficher_fnc(fnc);

    if (nb_cl_prec == 0) return true;

    if (existe_clause_fausse(fnc)) return false;

    //fnc = dpll_reduction(fnc);
    //nb_cl_prec = fnc.nb_clauses;

    //fnc = dpll_elim_litteraux_isoles(fnc);
    //if (nb_cl_prec != fnc.nb_clauses) return dpll_solver(fnc, rep);
    //nb_cl_prec = fnc.nb_clauses;


    int val_v = dpll_assignation_variable(fnc, &fnc_v, true);
    bool res_v = dpll_solver(fnc_v, rep);
    if (res_v && !existe_clause_fausse(fnc_v))
        rep->valeurs[rep->taille++] = val_v; 

    int val_f = dpll_assignation_variable(fnc, &fnc_f, false);
    bool res_f = dpll_solver(fnc_f, rep);
    if (res_f && !existe_clause_fausse(fnc_f))
        rep->valeurs[rep->taille++] = -val_f; 

    return res_v || res_f;
}
