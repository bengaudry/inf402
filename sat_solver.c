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
