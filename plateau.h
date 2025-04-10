#ifndef _PLATEAU_H_
#define _PLATEAU_H_

#include <stdbool.h>
#include "listes.h"
#include "commun.h"

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

/* Renvoie la dimension du plateau */
int dimension_plateau(Plateau P);

/* Renvoie le nombre de salles du plateau */
int nb_salles_plateau(Plateau P);

/* Renvoie la case du plateau d'indices (x, y) */
Case case_plateau(Plateau P, int x, int y);

/* Change la valeur de la case d'indices (x, y) par c */
void modifier_case(Plateau *P, int x, int y, Case c);

/* Renvoie une liste de cases voisines d'une flèche, sans la case pointée */
ListeCoor* cases_voisines_fleches(Plateau P, Coordonnees coor_fleche);

/* Renvoie la salle d'index i */
Salle salle_plateau(Plateau P, int i);

/* Ajoute une salle au plateau */
void ajouter_salle_plateau(Plateau *P, ListeCoor *liste_coor, int taille);

/* Renvoie l'index de la salle contenant la case c */
int index_salle_case(Case c, Plateau P);

/* Renvoie la salle contenant la case de coordonées (x, y) */
Salle salle_contenant_case(Plateau P, int x, int y);

/* Retourne true si les coordonées de la case sont correctes pour ce plateau */
bool case_dans_plateau(Plateau P, Coordonnees coor);

/* Affiche une erreur de plateau et termine le programme */
void erreur_plateau(ErreurPlateau err);

/* Lit un fichier plateau et crée un plateau avec les données lues */
ErreurPlateau lire_fichier_plateau(char *chemin, Plateau* P);

void afficher_plateau(Plateau P);

#endif
