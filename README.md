# Makaro

Sujet projet :
https://wackb.gricad-pages.univ-grenoble-alpes.fr/inf402/projet.pdf

Exemple makaro:
https://www.nikoli.co.jp/en/puzzles/makaro/

## Organisation du code

### Exécutables
Il y a trois fichiers exécutables dans le projet :

- **afficher_modelisation**

qui affiche la modélisation d'un plateau de jeu en forme normale conjonctive et crée le fichier dimacs associé.

```sh
make afficher_modelisation
./afficher_modelisation plateaux/<nom_du_plateau>.txt
```

- **afficher_solution**

qui modélise le plateau choisi en fnc, le convertit en format dimacs et à partir de la trace du sat solver, génère un plateau solution, ou affiche une erreur si le plateau est insatisfaisable.

```sh
make afficher_solution
./afficher_solution plateaux/<nom_du_plateau>.txt
```

- **jeu**

qui crée une partie interactive, niveau par niveau avec la possibilité de demander des indices générés par le sat solver.

```sh
make jeu
./jeu
```


### Tests
Les fichiers tests se trouve dans le dossier _tests/_ et peuvent être exécutés avec 

```sh
make test_<nom_du_test>
./test_<nom_du_test>
```

## Format plateau

Les plateaux sont décrits suivant la convention suivante dans un fichier texte et sont placés dans le répertoire _plateaux/_

1. d la dimension du plateau (dxd)
2. N le nombre de salles
Pour chaque salle avec i la taille de la salle :
3. i x1 y1 ... xi yi
4. La description du plateau case par case suivant ce format :
   - "."                 : Vide
   - "<", ">", "^", "v"  : flèches
   - 1 <= k <= d         : case initialisée avec le nombre k
    
