#ifndef _UTIL_H_
#define _UTIL_H_

#define DIM_MAX 99

#include <stdbool.h>

typedef enum { TypeFleche, TypeNombre, TypeVide } TypeCase;
typedef enum { Nord, Sud, Est, Ouest } Orientation;

typedef struct {
    int x;
    int y;
} Coordonnees;

typedef struct {
    Orientation or;
    Coordonnees case_pointee;
} Fleche;

Coordonnees creer_coor(int x, int y);
bool coor_egales(Coordonnees A, Coordonnees B);
Fleche creer_fleche(Orientation or, Coordonnees case_pointee);
Orientation get_orientation_fleche(Fleche f);
Coordonnees get_case_pointee_fleche(Fleche f);

#endif
