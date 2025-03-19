#ifndef _PLATEAU_H_
#define _PLATEAU_H_

#include "listes.h"
#include "utiles.h"

#define DIM_MAX 99

typedef enum { TypeFleche, TypeNombre, TypeVide } TypeCase;
typedef enum { Nord, Sud, Est, Ouest } Orientation;

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
    Orientation or;
    Coordonnees case_pointée;
} Fleche;

typedef struct {
    Fleche fleche;
    UINT nombre;
} ValeurCase;

typedef struct {
    TypeCase type;
    ValeurCase val;
    Coordonnees coor;
} Case;

typedef struct {
    ListeCoor *liste_coor;
    UINT taille;
} Salle;

typedef struct {
    Case cases[DIM_MAX][DIM_MAX];
    Salle salles[DIM_MAX];
    UINT nb_salles;
    UINT dim;
} Plateau;

Case get_case(Plateau P, UINT x, UINT y);

/* Retourne true si les coordonées de la case sont correctes pour ce plateau */
bool case_dans_plateau(Plateau P, Coordonnees coor);

/* Affiche une erreur de plateau et termine le programme */
void erreur_plateau(ErreurPlateau err);

/* Lit un fichier plateau et crée un plateau avec les données lues */
ErreurPlateau lire_fichier_plateau(char *chemin, Plateau* P);

void afficher_plateau(Plateau P);

#endif
