#ifndef _PLATEAU_H_
#define _PLATEAU_H_

#include <stdbool.h>
#include "listes.h"
#include "utiles.h"

typedef enum {
    OK,
    ErreurFichier,
    ErreurLecture,
    DimensionIncorrecte,
    NbSallesIncorrect,
    TailleSalleIncorrecte,
    FlechePointeDehors,
    SallesManquantes,
    NbCasesIncorrect,
    SallesMalformées,
    ErreurTypeCase,
    ErreurCoordonnees,
} ErreurPlateau;

typedef struct {
    Fleche fleche;
    int nombre;
} ValeurCase;

typedef struct {
    TypeCase type;
    ValeurCase val;
    Coordonnees coor;
} Case;

typedef struct {
    ListeCoor *liste_coor;
    int taille;
} Salle;

typedef struct {
    Case grille[DIM_MAX][DIM_MAX];
    Salle salles[DIM_MAX];
    int nb_salles;
    int dim;
} Plateau;

int get_dim_plateau(Plateau P);
int get_nb_salles_plateau(Plateau P);

Case get_case(Plateau P, int x, int y);
void set_case(Plateau *P, int x, int y, Case c);

Salle get_salle_plateau(Plateau P, int i);
void ajouter_salle_plateau(Plateau *P, ListeCoor *liste_coor, int taille);

/* Retourne true si les coordonées de la case sont correctes pour ce plateau */
bool case_dans_plateau(Plateau P, Coordonnees coor);

/* Affiche une erreur de plateau et termine le programme */
void erreur_plateau(ErreurPlateau err);

/* Lit un fichier plateau et crée un plateau avec les données lues */
ErreurPlateau lire_fichier_plateau(char *chemin, Plateau* P);

void afficher_plateau(Plateau P);

#endif
