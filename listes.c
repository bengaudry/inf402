#include "listes.h"
#include <stdlib.h>

ListeCoor* init_liste_coor() {
    ListeCoor* l;

    l = malloc(sizeof(ListeCoor));
    l->first = NULL;
    l->last = NULL;
    l->taille = 0;

    return l;
}

void ajouter_element_liste_coor(ListeCoor* l, Coordonnees coor) {
    CellListCoor *cell;

    cell = malloc(sizeof(CellListCoor));

    cell->coor = coor;
    cell->suiv = NULL;
    
    if (l->first == NULL) {
        l->first = cell;
        l->last = cell;
        l->taille = 1;
        return;
    }

    l->last->suiv = cell;
    l->last = cell;
    l->taille = l->taille + 1;
}

UINT taille_liste_coor(ListeCoor* l) {
    return l->taille;
}
