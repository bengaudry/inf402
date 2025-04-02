# Makaro

Sujet projet :
https://wackb.gricad-pages.univ-grenoble-alpes.fr/inf402/projet.pdf

Exemple makaro:
https://www.nikoli.co.jp/en/puzzles/makaro/

## Organisation du code

### Tests
Les fichiers tests se trouve dans le dossier _tests/_ et peuvent être exécutés avec 

```sh
make test_<nom_du_test>
./test_<nom_du_test>
```

## Format plateau

// d la dimension du plateau (dxd)
1. d
// N le nombre de salles
2. N
// pour chaque salle :
// i la taille de la salle
3. i x1 y1 ... xi yi
// Le plateau de base
4. ..<..
   ....3
   ....<
   >....
   ..<..

.          : Vide
<, >, ^, v : flèches
k          : nombre dans case
    
