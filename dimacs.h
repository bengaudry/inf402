#ifndef _DIMACS_H_
#define _DIMACS_H_

#include "logique.h"


/*créer un entier associé à une variable logique
  dim: dimension grille
  maxVal: valeur maximale de la grille           */
int encodage_id(int dim, int maxVal, VarLogique var);

/*traduit un id en valeur logique*/
void decodage_id(int dim, int maxVal, int id, int* x, int* y, int* val);

void sortie_dimacs(FNC fnc, int dim, int maxVal, char *fichier_sortie);

#endif