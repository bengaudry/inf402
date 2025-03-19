#include "listes.h"
#include "utiles.h"
#include <stdio.h>

int main(void) {
    Coordonnees c;
    ListeCoor* l = init_liste_coor();

    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 2) : %s\n\n", liste_coor_contient(l, creer_coor(1, 2)) ? "Oui" : "Non");

    c.x= 1;
    c.y= 2;
    ajouter_element_liste_coor(l, c);

    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 2) : %s\n\n", liste_coor_contient(l, creer_coor(1, 2)) ? "Oui" : "Non");

    c.x= 4;
    c.y= 5;
    ajouter_element_liste_coor(l, c);
    c.x= 3;
    c.y= 2;
    ajouter_element_liste_coor(l, c);
    c.x= 1;
    c.y= 3;
    ajouter_element_liste_coor(l, c);
    
    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 3) : %s\n", liste_coor_contient(l, creer_coor(1, 3)) ? "Oui" : "Non");
    printf("Contient (1, 0) : %s\n", liste_coor_contient(l, creer_coor(1, 0)) ? "Oui" : "Non");

    return 0;
}
