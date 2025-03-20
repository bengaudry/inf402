#ifndef _LISTES_H_
#define _LISTES_H_

#include <stdbool.h>
#include "utiles.h"

typedef struct _s_cell_ {
    Coordonnees coor;
    struct _s_cell_ *suiv;
} CellListCoor;

typedef struct {
    CellListCoor *first;
    CellListCoor *last;
    int taille;
} ListeCoor;

ListeCoor* init_liste_coor();

void ajouter_element_liste_coor(ListeCoor* l, Coordonnees coor);

bool liste_coor_contient(ListeCoor* l, Coordonnees coor);

int taille_liste_coor(ListeCoor* l);

void afficher_liste_coor(ListeCoor* l);

#endif
