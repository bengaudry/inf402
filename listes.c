#include <stdlib.h>
#include <stdio.h>
#include "listes.h"

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

bool liste_coor_contient(ListeCoor* l, Coordonnees coor) {
    CellListCoor* cell = l->first;

    while (cell != NULL) {
        if (cell->coor.x == coor.x && cell->coor.y == coor.y) {
            return true;
        }
        cell = cell->suiv;
    }

    return false;
}

UINT taille_liste_coor(ListeCoor* l) {
    return l->taille;
}

void afficher_liste_coor(ListeCoor* l) {
    CellListCoor* cell = l->first;

    printf("[");
    while (cell != NULL && cell->suiv != NULL) {
        printf("(%d, %d), ", cell->coor.x, cell->coor.y);
        cell = cell->suiv;
    }
    if (cell != NULL) {
        printf("(%d, %d)", cell->coor.x, cell->coor.y);
    }
    printf("]\n");
}
