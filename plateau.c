#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>

#include "utiles.h"
#include "plateau.h"
#include "listes.h"

Case get_case(Plateau P, UINT x, UINT y) {
    return P.cases[x][y];
}

/* Retourne true si les coordonées de la case sont correctes pour ce plateau */
bool case_dans_plateau(Plateau P, Coordonnees coor) {
    if (coor.x < 1 || coor.x > P.dim) return false;
    if (coor.y < 1 || coor.y > P.dim) return false;
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
    UINT dim, nb_salles;
    UINT nb_fleches = 0, total_cases_salles = 0, nb_cases_total;
    
    f_plateau = fopen(chemin, "r");
    if (f_plateau == NULL) return ErreurFichier;

    // Lecture de la dimension du plateau
    fscanf(f_plateau, "%d", &dim);
    if (dim < 2 || dim > 9) return DimensionIncorrecte;
    P->dim = dim;
    
    // Lecture du nombre de salles du plateau
    fscanf(f_plateau, "%d", &nb_salles);
    if (nb_salles < 2 || nb_salles > dim*dim) return NbSallesIncorrect;
    P->nb_salles = nb_salles;

    // Lecture des salles
    for (int i = 1; i <= nb_salles; i++) {
        UINT taille_salle;
        ListeCoor *liste_coor_salles = init_liste_coor();

        if (!fscanf(f_plateau, "%d", &taille_salle)) return SallesManquantes;
        if (taille_salle < 1 || taille_salle > dim*dim) return TailleSalleIncorrecte;

        for (int j = 1; j <= taille_salle; j++) {
            Coordonnees coor;
            if (!fscanf(f_plateau, "%d %d", &coor.x, &coor.y)) return ErreurCoordonnees;
            if (!case_dans_plateau(*P, coor)) return ErreurCoordonnees;
            if (coor.x < 1 || coor.x > dim || coor.y < 1 || coor.y > dim) return ErreurCoordonnees;
            total_cases_salles++;
            ajouter_element_liste_coor(liste_coor_salles, coor);
        }

        P->salles[i].liste_coor = liste_coor_salles;
    }

    // Récupération des cases
    for (int y = 1; y <= dim; y++) {
        for (int x = 1; x <= dim; x++) {
            Case c;
            ValeurCase val_case;
            Fleche fleche;
            Coordonnees coor;
            char type_case;

            do {
                if (!fscanf(f_plateau, "%c", &type_case)) return ErreurLecture;
            } while (type_case == '\n' || type_case == ' ');

            nb_cases_total++;
            switch (type_case) {
                case '.':
                    c.type = TypeVide;
                    break;
                // Gestion des flèches
                case '<':
                    nb_fleches++;
                    c.type = TypeFleche;
                    fleche.or = Ouest;
                    coor = creer_coor(x-1, y);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche.case_pointée = coor;
                    val_case.fleche = fleche;
                    break;
                case '>':
                    nb_fleches++;
                    c.type = TypeFleche;
                    fleche.or = Est;
                    coor = creer_coor(x+1, y);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche.case_pointée = coor;
                    val_case.fleche = fleche;
                    break;
                case 'v':
                    nb_fleches++;
                    c.type = TypeFleche;
                    fleche.or = Sud;
                    coor = creer_coor(x, y+1);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche.case_pointée = coor;
                    val_case.fleche = fleche;
                    break;
                case '^':
                    nb_fleches++;
                    c.type = TypeFleche;
                    fleche.or = Nord;
                    coor = creer_coor(x, y-1);
                    if (!case_dans_plateau(*P, coor)) return FlechePointeDehors;
                    fleche.case_pointée = coor;
                    val_case.fleche = fleche;
                    break;
                default:
                    if (isdigit(type_case)) {
                        c.type = TypeNombre;
                        val_case.nombre = type_case - '0';
                    } else {
                        return ErreurTypeCase;
                    }
                    break;
            }
            c.val = val_case;
            c.coor.x = x;
            c.coor.y = y;
            P->cases[x][y] = c;
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
    UINT dim, nb_salles;

    dim = P.dim;
    nb_salles = P.nb_salles;

    printf("Dim = %d; Nb_salles = %d\n", dim, nb_salles);

    for (int y = 1; y <= dim; y++) {
        afficher_ligne(dim);
        printf("|");
        for (int x = 1; x <= dim; x++) {
            Case c = get_case(P, x, y);
            switch (c.type) {
                case TypeVide:
                    printf("   |");
                    break;
                case TypeFleche:
                    if (c.val.fleche.or == Ouest) {
                        printf(" < |");
                    } else if (c.val.fleche.or == Est) {
                        printf(" > |");
                    } else if (c.val.fleche.or == Sud) {
                        printf(" v |");
                    } else if (c.val.fleche.or == Nord) {
                        printf(" ^ |");
                    }
                    break;
                case TypeNombre:
                    printf(" %d |", c.val.nombre);
                    break;
            }
        }
        printf("\n");
    }
    afficher_ligne(dim);
}
