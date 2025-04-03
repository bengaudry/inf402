#include <stdlib.h>
#include <stdio.h>
#include "logique.h"
#include "plateau.h"
#include "commun.h"

/* Ajoute à la fnc les clauses générées par la règle 1 sur les cases voisines */
void modeliser_regle_cases_voisines(FNC* fnc, Plateau P) {
    int x, y;
    unsigned int dim;
    Case c;
    Salle salle;
    VarLogique var_log;
    Clause cl;

    dim = dimension_plateau(P);

    for (x = 1; x <= dim; x++) {
        for (y = 1; y <= dim; y++) {
            c = case_plateau(P, x, y);
            switch (c.type) {
                case TypeFleche: break; // Pas de règle 1 si c'est une flèche
                case TypeVide:
                    // Note: Faire quasi pareil que pour les cases déjà initialisées mais
                    // au lieu d'utiliser la valeur de la case, utiliser toutes les valeurs
                    // possibles de 1 à la taille de la salle
                    break;
                case TypeNombre:
                    // Case déjà initialisée
                    cl = initialiser_clause();
                    int val_case = c.val.nombre;

                    // Ajout de la valeur initiale
                    ajouter_variable_a_clause(&cl, creer_var_logique(val_case, x, y, false));
                    ajouter_clause_a_fnc(fnc, cl);

                    // Les cases voisines ne doivent pas avoir la même valeur 
                    // (si elles sont dans le plateau, et que ce ne sont pas des flèches)
                    if (case_dans_plateau(P, creer_coor(x+1, y)) && case_plateau(P, x+1, y).type != TypeFleche) {
                        cl = initialiser_clause();
                        ajouter_variable_a_clause(&cl, creer_var_logique(val_case, x+1, y, true));
                        ajouter_clause_a_fnc(fnc, cl);  
                    }
                    if (case_dans_plateau(P, creer_coor(x, y+1)) && case_plateau(P, x, y+1).type != TypeFleche) {
                        cl = initialiser_clause();
                        ajouter_variable_a_clause(&cl, creer_var_logique(val_case, x, y+1, true));
                        ajouter_clause_a_fnc(fnc, cl);  
                    }
                    if (case_dans_plateau(P, creer_coor(x, y-1)) && case_plateau(P, x, y-1).type != TypeFleche) {
                        cl = initialiser_clause();
                        ajouter_variable_a_clause(&cl, creer_var_logique(val_case, x, y-1, true));
                        ajouter_clause_a_fnc(fnc, cl);  
                    }
                    if (case_dans_plateau(P, creer_coor(x-1, y)) && case_plateau(P, x-1, y).type != TypeFleche) {
                        cl = initialiser_clause();
                        ajouter_variable_a_clause(&cl, creer_var_logique(val_case, x-1, y, true));
                        ajouter_clause_a_fnc(fnc, cl);  
                    }
                    break;
            }

        }
    }
}

/* Ajoute à la fnc les clauses générées par la règle 2 sur le remplissage des salles */
void modeliser_regle_remplissage_salles(FNC* fnc, Plateau P) {
    int idx_salle, nb_salles, taille_salle;
    Clause cl, cl_unicite;
    CellListCoor *cel1, *cel2;

    nb_salles = nb_salles_plateau(P);

    for (idx_salle = 0; idx_salle < nb_salles; idx_salle++) { // pour toute salle
        Salle S = salle_plateau(P, idx_salle);
        taille_salle = S.taille;

        int k;
        for (k = 1; k <= taille_salle; k++) { // pour tout entier entre 1 et |C(s)|
            cl = initialiser_clause();
            
            cel1 = S.liste_coor->first;
            while (cel1 != NULL) { // pour toute case c1 dans s
                ajouter_variable_a_clause(&cl, creer_var_logique(k, cel1->coor.x, cel1->coor.y, false));

                cel2 = cel1->suiv; // on commence à la case qui suit c1 pour éviter les doublons
                while (cel2 != NULL) { // pour toute case c2 != c1 dans s
                    cl_unicite = initialiser_clause();
                    ajouter_variable_a_clause(&cl_unicite, creer_var_logique(k, cel1->coor.x, cel1->coor.y, true));
                    ajouter_variable_a_clause(&cl_unicite, creer_var_logique(k, cel2->coor.x, cel2->coor.y, true));
                    ajouter_clause_a_fnc(fnc, cl_unicite);
                    cel2 = cel2->suiv;
                }

                cel1 = cel1->suiv;
            }
            ajouter_clause_a_fnc(fnc, cl);
        }
    }
}

/* Ajoute à la fnc les clauses générées par la règle 3 sur les flèches */
void modeliser_regle_fleches(FNC* fnc, Plateau P) {
    return;
}

/* Modélise chaque règle du jeu en fnc */
FNC* modeliser_jeu(Plateau P) {
    FNC *fnc;

    fnc = initialiser_FNC();
    modeliser_regle_cases_voisines(fnc, P);
    modeliser_regle_remplissage_salles(fnc, P);
    modeliser_regle_fleches(fnc, P);

    return fnc;
}
