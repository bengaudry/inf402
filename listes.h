#ifndef _LISTES_H_
#define _LISTES_H_

#include <stdbool.h>
#include "commun.h"

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

/* Renvoie true si l contient les coordonées coor */
bool liste_coor_contient(ListeCoor* l, Coordonnees coor);

/* Renvoie la taille de la liste de coordonées l */
int taille_liste_coor(ListeCoor* l);

void afficher_liste_coor(ListeCoor* l);

#endif
