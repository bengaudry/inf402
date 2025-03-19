#ifndef _TYPES_H_
#define _TYPES_H_

typedef unsigned int UINT;

typedef struct {
    UINT x;
    UINT y;
} Coordonnees;

Coordonnees creer_coor(UINT x, UINT y);

#endif
