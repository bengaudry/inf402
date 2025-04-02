#ifndef _MODELISATION_H_
#define _MODELISATION_H_

#include "logique.h"
#include "plateau.h"

/* Ajoute à la fnc les clauses générées par la règle 1 sur les cases voisines */
void modeliser_regle_cases_voisines(FNC* fnc, Plateau P);

/* Ajoute à la fnc les clauses générées par la règle 2 sur le remplissage des salles */
void modeliser_regle_remplissage_salles(FNC* fnc, Plateau P);

/* Ajoute à la fnc les clauses générées par la règle 3 sur les flèches */
void modeliser_regle_fleches(FNC* fnc, Plateau P);

/* Modélise chaque règle du jeu en fnc */
FNC* modeliser_jeu(Plateau P);

#endif
