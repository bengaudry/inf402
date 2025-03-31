#include "commun.h"

////// COORDONNÉES //////
Coordonnees creer_coor(int x, int y) {
    Coordonnees coor;
    coor.x = x;
    coor.y = y;
    return coor;
}

bool coor_egales(Coordonnees A, Coordonnees B) {
    return A.x == B.x && A.y == B.y;
}


////// FLÈCHES //////
Fleche creer_fleche(Orientation or, Coordonnees case_pointee) {
    Fleche f;
    f.or = or;
    f.case_pointee = case_pointee;
    return f;
}

Orientation orientation_fleche(Fleche f) {
    return f.or;
}

Coordonnees case_pointee_fleche(Fleche f) {
    return f.case_pointee;
}
