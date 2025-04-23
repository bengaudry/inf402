#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "plateau.h"
#include "logique.h"
#include "modelisation.h"
#include "dimacs.h"
#include "commun.h"
#include "minisat.h"

int nombre_cases_pleines(Plateau P){
    int nb_cases_pleines = 0;
    Case c;

    for (int x=1; x<=dimension_plateau(P); x++) {
        for (int y=1; y<=dimension_plateau(P); y++) {
            c = case_plateau(P, x, y);
            if (c.type != TypeVide) {
                nb_cases_pleines++;
            }
        }
    }
    return nb_cases_pleines;
}

bool fleches_egales(Fleche f1, Fleche f2) {
    return (coor_egales(f1.case_pointee, f2.case_pointee) && f1.or == f2.or);
}

bool cases_egales(Case c1, Case c2) {
    if (c1.type != c2.type) return false;

    if (c1.type == TypeNombre) {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type && c1.val.nombre == c2.val.nombre);
    }
    else if (c1.type == TypeVide) {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type);
    }
    else {
        return (coor_egales(c1.coor, c2.coor) && c1.type == c2.type && fleches_egales(c1.val.fleche, c2.val.fleche));
    }
}

bool plateaux_egaux(Plateau P1, Plateau P2) {
    int dim = dimension_plateau(P1);

    for (int x=1; x<=dim; x++) {
        for (int y=1; y<=dim; y++) {
            if (cases_egales(case_plateau(P1, x, y), case_plateau(P2, x, y)) == false) {
                return false;
            }
        }
    }
    return true;
}

int demander_choix_joueur() {
    int choix = -1;
    do {
        printf("\n(1) pour remplir une case\n");
        printf("(2) pour demander un indice\n");
        printf("(3) pour abandonner\n");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2 && choix != 3);
    return choix;
}

void attendre() {
    int c;
    printf("Appuyez sur n'importe quelle touche pour continuer...\n");
    
    // Vider le buffer
    while ((c = getchar()) != '\n' && c != EOF);

    // Attendre un nouvel appui
    getchar();
}

bool jouer_plateau(char* fichier_plateau, unsigned int niv) {
    system("clear");

    /*----- LECTURE PLATEAU -----*/
    Plateau P, P_joueur;
    ErreurPlateau err;

    err = lire_fichier_plateau(fichier_plateau, &P);
    if (err != OK) erreur_plateau(err);
    err = lire_fichier_plateau(fichier_plateau, &P_joueur);

    /*----- JOUER PARTIE -----*/
    int x, y;
    unsigned int dim = dimension_plateau(P);
    int nb_cases_total = dim*dim;
    int nb_cases_pleines = nombre_cases_pleines(P_joueur);
    Case c;
    c.type = TypeNombre;
    int choix;

    //boucle de jeu
    while (nb_cases_pleines < nb_cases_total) {
        system("clear");
        printf(">---- NIVEAU %d ----<\n", niv);
        afficher_plateau(P_joueur);
        choix = demander_choix_joueur();

        // Remplissage d'une case
        if (choix == 1) {
            printf("\nEntrez une case: x y valeur:\n");
            scanf("%d %d %d", &c.coor.x, &c.coor.y, &c.val.nombre);
            
            //verifier les bonnes coordonnees
            while (case_dans_plateau(P_joueur, creer_coor(c.coor.x, c.coor.y)) == false) {
                printf("\nCoordonnees incorrectes : 1 <= x,y <= %d\n", dimension_plateau(P_joueur));
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &c.coor.x, &c.coor.y);
            }

            //verifier que la case peut recevoir un nombre : != typeFleche
            while (case_plateau(P_joueur, c.coor.x, c.coor.y).type == TypeFleche) {
                printf("\nCette case contient une flèche\n");
                printf("Entrez de nouvelles coordonnees: x y\n");
                scanf("%d %d", &c.coor.x, &c.coor.y);
            }            

            //on incrémente si la case etait vide
            if (case_plateau(P_joueur, c.coor.x, c.coor.y).type == TypeVide) nb_cases_pleines++;
            //modification de la case
            modifier_case(&P_joueur, c.coor.x, c.coor.y, c);

            P = P_joueur;
        } 

        // Donner un indice
        if (choix == 2) {
            if (!generer_solution(P_joueur, &P)) {
                printf("\nVotre jeu n'admet pas de solution tel quel, essayez de modifier la valeur de certaines cases...\n\n");
                attendre();
            } else {

                printf("\nEntrez une case à dévoiler: x y\n");
                scanf("%d %d", &x, &y);

                //verifier les bonnes coordonnees
                while (case_dans_plateau(P_joueur, creer_coor(x, y)) == false) {
                    printf("\nCoordonnees incorrectes : 1 <= x,y <= %d\n", dimension_plateau(P_joueur));
                    printf("Entrez de nouvelles coordonnees: x y\n");
                    scanf("%d %d", &x, &y);
                }

                //verifier que la case contienne un nombre
                while (case_plateau(P, x, y).type != TypeNombre) {
                    printf("\nCette case contient une flèche\n");
                    printf("Entrez de nouvelles coordonnees: x y\n");
                    scanf("%d %d", &x, &y);
                }
                //si la case etait vide on incremente
                if (case_plateau(P_joueur, x, y).type == TypeVide) nb_cases_pleines++;
                //modification de la case indice choisie
                modifier_case(&P_joueur, x, y, case_plateau(P, x, y));
            }
        }

        // Abandon de la partie
        if (choix == 3) break;
    }

    bool existe_solution = generer_solution(P_joueur, &P);
    if (existe_solution && plateaux_egaux(P_joueur, P)) {
        printf("\n\nBRAVO, niveau réussi\n");
        return true;
    }

    printf("\n\nNiveau échoué... voici un plateau réponse\n");
    afficher_plateau(P);
    return false;
}

int demander_si_joueur_veut_continuer() {
    int choix = -1;
    do {
        printf("\n(1) pour jouer le niveau suivant\n");
        printf("(2) pour rejouer ce niveau\n");
        printf("(3) pour arrêter de jouer\n");
        scanf("%d", &choix);
    } while (choix != 1 && choix != 2 && choix != 3);
    return choix;
}

//arg1: chemin_fichier_plateau
int main(int argc, char **argv) {
    int choix = 2;
    unsigned int nb_succes = 0;
    unsigned int nb_parties = 0;
    int niv_courant = 2;

    do {
        nb_parties++;
        char nom_plateau[100];
        sprintf(nom_plateau, "plateaux/%dx%d.txt", niv_courant, niv_courant);
        if (jouer_plateau(nom_plateau, niv_courant-1)) nb_succes++;
        choix = demander_si_joueur_veut_continuer();
        if (choix == 1) niv_courant++;
    } while ((choix == 1 || choix == 2) && niv_courant <= 6);

    printf("> %d victoires / %d défaites\n", nb_succes, nb_parties-nb_succes);

    return 0;
}
