#ifndef _DIMACS_H_
#define _DIMACS_H_

#include "logique.h"


#ifdef ENCODAGE_V1

/*créer un entier associé à une variable logique
  dim: dimension grille
  maxVal: valeur maximale de la grille           */
int encodage_id(int dim, int maxVal, VarLogique var);

/*traduit un id en valeur logique*/
void decodage_id(int dim, int maxVal, int id, int* x, int* y, int* val);
#endif

#ifndef ENCODAGE_V1
int encodage_id(FNC fnc, VarLogique var);
VarLogique decodage_id(FNC fnc, int id);
#endif

void sortie_dimacs(FNC fnc, int dim, int maxVal, char *fichier_sortie);

#endif