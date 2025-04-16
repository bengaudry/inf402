#ifndef _DIMACS_H_
#define _DIMACS_H_

#include "logique.h"

int encodage_id(FNC fnc, VarLogique var);
VarLogique decodage_id(FNC fnc, int id);

void sortie_dimacs(FNC fnc, int dim, int maxVal, char *fichier_sortie);

#endif