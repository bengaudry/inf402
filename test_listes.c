#include "listes.h"
#include "utiles.h"
#include <stdio.h>

int main(void) {
    Coordonnees c;
    ListeCoor* l = init_liste_coor();

    c = creer_coor(1, 2);
    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 2) : %s\n\n", liste_coor_contient(l, c) ? "Oui" : "Non");

    ajouter_element_liste_coor(l, c);

    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 2) : %s\n\n", liste_coor_contient(l, c) ? "Oui" : "Non");

    c = creer_coor(4, 5);
    ajouter_element_liste_coor(l, c);
    c = creer_coor(3, 2);
    ajouter_element_liste_coor(l, c);
    c = creer_coor(1, 3);
    ajouter_element_liste_coor(l, c);
    
    afficher_liste_coor(l);
    printf("Taille : %d\n", taille_liste_coor(l));
    printf("Contient (1, 3) : %s\n", liste_coor_contient(l, creer_coor(1, 3)) ? "Oui" : "Non");
    printf("Contient (1, 0) : %s\n", liste_coor_contient(l, creer_coor(1, 0)) ? "Oui" : "Non");

    return 0;
}
