#include "utiles.h"

Coordonnees creer_coor(int x, int y) {
    Coordonnees coor;
    coor.x = x;
    coor.y = y;
    return coor;
}

bool coor_egales(Coordonnees A, Coordonnees B) {
    return A.x == B.x && A.y == B.y;
}

Fleche creer_fleche(Orientation or, Coordonnees case_pointee) {
    Fleche fleche;
    fleche.or = or;
    fleche.case_pointee = case_pointee;
    return fleche;
}

Orientation get_orientation_fleche(Fleche f) {
    return f.or;
}

Coordonnees get_case_pointee_fleche(Fleche f) {
    return f.case_pointee;
}
