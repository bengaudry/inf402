#ifndef _LISTES_H_
#define _LISTES_H_

#include "utiles.h"

typedef struct _s_cell_ {
    Coordonnees coor;
    struct _s_cell_ *suiv;
} CellListCoor;

typedef struct {
    CellListCoor *first;
    CellListCoor *last;
    UINT taille;
} ListeCoor;

ListeCoor* init_liste_coor();

void ajouter_element_liste_coor(ListeCoor* l, Coordonnees coor);

UINT taille_liste_coor(ListeCoor* l);

#endif
