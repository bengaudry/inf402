#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "utiles.h"
#include "plateau.h"
#include "listes.h"

char couleurs_shell[7][9] = {
    "\033[30;41m",
    "\033[30;42m",
    "\033[30;43m",
    "\033[30;44m",
    "\033[30;45m",
    "\033[30;46m",
    "\033[30;47m"
};

char *couleur_defaut_shell = "\033[0m";

/* Renvoie la dimension du plateau */
int dimension_plateau(Plateau P) {
    return P.dim;
}

/* Renvoie le nombre de salles du plateau */
int nb_salles_plateau(Plateau P) {
    return P.nb_salles;
}

/* Renvoie la case du plateau d'indices (x, y) */
Case case_plateau(Plateau P, int x, int y) {
    return P.grille[x][y];
}

/* Change la valeur de la case d'indices (x, y) par c */
void modifier_case(Plateau *P, int x, int y, Case c) {
    P->grille[x][y] = c;
}

/* Renvoie la salle d'index i */
Salle salle_plateau(Plateau P, int i) {
    return P.salles[i];
}

/* Ajoute une salle au plateau */
void ajouter_salle_plateau(Plateau *P, ListeCoor *liste_coor, int taille) {
    Salle s;
    int nb_salles;

    nb_salles = nb_salles_plateau(*P);
    s.liste_coor = liste_coor;
    s.taille = taille;

    P->salles[nb_salles] = s;
    P->nb_salles++;
}

/* Renvoie l'index de la salle contenant la case c */
int index_salle_case (Case c, Plateau P) {
    CellListCoor *cel;
    int idx_salle, i, nb_salles;

    nb_salles = nb_salles_plateau(P);

    for (idx_salle = 0; idx_salle < nb_salles; idx_salle++) {
        Salle s = salle_plateau(P, idx_salle);

        for (i = 0; i < s.taille; i++) {
            cel = s.liste_coor->first;
            while (cel != NULL) {
                if (coor_egales(cel->coor, c.coor)) return idx_salle;
                cel = cel->suiv;
            }
        }
    }

    return -1; // Case n'est pas dans une salle (flèche)
}

/* Retourne true si les coordonées de la case sont correctes pour ce plateau */
bool case_dans_plateau(Plateau P, Coordonnees coor) {
    int dim = dimension_plateau(P);
    if (coor.x < 1 || coor.x > dim) return false;
    if (coor.y < 1 || coor.y > dim) return false;
    return true;
}

/* Affiche une erreur de plateau et termine le programme */
void erreur_plateau(ErreurPlateau err) {
    if (err == OK) return;

    switch (err) {
        case ErreurFichier:
            fprintf(stderr, "Erreur: Impossible d'ouvrir le fichier\n");
            break;
        case ErreurLecture:
            fprintf(stderr, "Erreur: Impossible de lire le fichier\n");
            break;
        case DimensionIncorrecte:
            fprintf(stderr, "Erreur: Dimension incorrecte\n");
            break;
        case NbSallesIncorrect:
            fprintf(stderr, "Erreur: Nombre de salles incorrect\n");
            break;
        case SallesManquantes:
            fprintf(stderr, "Erreur: Salles manquantes\n");
            break;
        case FlechePointeDehors:
            fprintf(stderr, "Erreur: Fleche pointe en dehors du plateau\n");
            break;
        case SallesMalformées:
            fprintf(stderr, "Erreur: Salles malformées\n");
            break;
        case NbCasesIncorrect:
            fprintf(stderr, "Erreur: Nombre de cases incorrect\n");
            break;
        case TailleSalleIncorrecte:
            fprintf(stderr, "Erreur: Taille de salle incorrecte\n");
            break;
        case ErreurTypeCase:
            fprintf(stderr, "Erreur: Type de case incorrect\n");
            break;
        case ErreurCoordonnees:
            fprintf(stderr, "Erreur: Coordonnées incorrectes\n");
            break;
        default:
            break;
    }
    exit(1);
}

/* Lit un fichier plateau et crée un plateau avec les données lues */
ErreurPlateau lire_fichier_plateau(char *chemin, Plateau *P) {
    FILE *f_plateau;
    int dim, nb_salles;
    int nb_fleches = 0, nb_cases_initialisees = 0, total_cases_salles = 0, nb_cases_total = 0;
    Coordonnees coor;
    
    f_plateau = fopen(chemin, "r");
    if (f_plateau == NULL) return ErreurFichier;


    // Lecture de la dimension du plateau
    fscanf(f_plateau, "%d", &dim);
    if (dim < 2 || dim > DIM_MAX) return DimensionIncorrecte;
    P->dim = dim;
    
    // Lecture du nombre de salles du plateau
    fscanf(f_plateau, "%d", &nb_salles);
    if (nb_salles < 1 || nb_salles > dim*dim) return NbSallesIncorrect;
    P->nb_salles = 0; // Le nombre de salles est automatiquement incrémenté dans la fonction ajouter_salle_plateau

    // Lecture des salles
    for (int i = 1; i <= nb_salles; i++) {
        int taille_salle;
        ListeCoor *liste_coor_salles = init_liste_coor();

        if (!fscanf(f_plateau, "%d", &taille_salle)) return SallesManquantes;
        if (taille_salle < 1 || taille_salle > dim*dim) return TailleSalleIncorrecte;

        for (int j = 1; j <= taille_salle; j++) {
            if (!fscanf(f_plateau, "%d %d", &coor.x, &coor.y)) return ErreurCoordonnees;
            if (!case_dans_plateau(*P, coor)) return ErreurCoordonnees;
            if (coor.x < 1 || coor.x > dim || coor.y < 1 || coor.y > dim) return ErreurCoordonnees;
            total_cases_salles++;
            ajouter_element_liste_coor(liste_coor_salles, coor);
        }

        ajouter_salle_plateau(P, liste_coor_salles, taille_salle);
    }

    // Lecture des cases
    Case c;
    ValeurCase val_case;
    Fleche fleche;
    char type_case;
    
    for (int y = 1; y <= dim; y++) {
        for (int x = 1; x <= dim; x++) {

            do {
                if (!fscanf(f_plateau, "%c", &type_case)) return ErreurLecture;
            } while (type_case == '\n' || type_case == ' ');

            nb_cases_total++;
            switch (type_case) {
                // Gestion des flèches
                case '<':
                    nb_fleches++;
                    c.type = TypeFleche;
                    coor = creer_coor(x-1, y);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche = creer_fleche(Ouest, coor);
                    val_case.fleche = fleche;
                    break;
                case '>':
                    nb_fleches++;
                    c.type = TypeFleche;
                    coor = creer_coor(x+1, y);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche = creer_fleche(Est, coor);
                    val_case.fleche = fleche;
                    break;
                case 'v':
                    nb_fleches++;
                    c.type = TypeFleche;
                    coor = creer_coor(x, y+1);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche = creer_fleche(Sud, coor);
                    val_case.fleche = fleche;
                    break;
                case '^':
                    nb_fleches++;
                    c.type = TypeFleche;
                    coor = creer_coor(x, y-1);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche = creer_fleche(Nord, coor);
                    val_case.fleche = fleche;
                    break;

                // Case vide
                case '.':
                    c.type = TypeVide;
                    break;

                default:
                    // Type de case inconnu
                    if (!isdigit(type_case)) return ErreurTypeCase;
                    
                    // Case initialisée
                    c.type = TypeNombre;
                    val_case.nombre = type_case - '0';
                    nb_cases_initialisees++;
                    break;
            }
            c.val = val_case;
            c.coor = creer_coor(x, y);
            modifier_case(P, x, y, c);
        }
    }
    
    if (nb_cases_total != dim*dim) return NbCasesIncorrect;
    if (nb_cases_total - nb_fleches != total_cases_salles) return SallesMalformées;
    
    fclose(f_plateau);
    return OK;
}

void afficher_ligne(int dim) {
    for (int i = 0; i < dim; i++) {
        printf("+---");
    }
    printf("+\n");
}

void afficher_plateau(Plateau P) {
    int dim, nb_salles, idx;
    Orientation or_fleche;

    dim = dimension_plateau(P);
    nb_salles = nb_salles_plateau(P);

    printf("Dim = %d; Nb_salles = %d\n", dim, nb_salles);

    for (int y = 1; y <= dim; y++) {
        afficher_ligne(dim);
        printf("|");
        for (int x = 1; x <= dim; x++) {
            Case c = case_plateau(P, x, y);
            switch (c.type) {
                case TypeVide:
                    // Affiche une couleur de fond correspondant à la salle
                    idx = index_salle_case(c, P);
                    printf("%s   %s|", couleurs_shell[idx%7], couleur_defaut_shell);
                    break;
                case TypeFleche:
                    or_fleche = orientation_fleche(c.val.fleche);
                    printf("\033[1m "); // gras
                    switch (or_fleche) {
                        case Ouest:
                            printf("←");
                            break;
                        case Est:
                            printf("→");
                            break;
                        case Sud:
                            printf("↓");
                            break;
                        case Nord:
                            printf("↑");
                            break;
                    }
                    printf("\033[0m |");
                    break;
                case TypeNombre:
                    // Affiche une couleur de fond correspondant à la salle
                    idx = index_salle_case(c, P);
                    printf("\033[1m%s %d %s|", couleurs_shell[idx%7], c.val.nombre, couleur_defaut_shell);
                    break;
            }
        }
        printf("\n");
    }
    afficher_ligne(dim);
}
