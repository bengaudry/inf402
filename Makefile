CC=clang
CFLAGS=-Werror

EXECUTABLES = test_plateau test_listes test_logique test_modelisation test_export_dimacs test_sat_solver main afficher_modelisation jeu

all: $(EXECUTABLES)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<


# ====== MINISAT ===== #
minisat.o: minisat_src/minisat.c minisat_src/minisat.h
	$(CC) -c $<

solver.o: minisat_src/solver.c minisat_src/solver.h
	$(CC) -c $<
# ====== MINISAT ===== #


main: main.c plateau.o listes.o commun.o logique.o minisat.o solver.o modelisation.o dimacs.o sat_solver.o
	$(CC) $(CFLAGS) -lm -o $@ $^

afficher_modelisation: afficher_modelisation.c plateau.o listes.o commun.o logique.o modelisation.o dimacs.o
	$(CC) $(CFLAGS) -lm -o $@ $^

jeu: jeu.c plateau.o listes.o commun.o logique.o minisat.o solver.o modelisation.o dimacs.o
	$(CC) $(CFLAGS) -lm -o $@ $^

# ===== TESTS ===== #

test_plateau: tests/test_plateau.c plateau.o listes.o commun.o logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_listes: tests/test_listes.c plateau.o listes.o commun.o logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_logique: tests/test_logique.c logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_modelisation: tests/test_modelisation.c modelisation.o plateau.o logique.o commun.o listes.o
	$(CC) $(CFLAGS) -o $@ $^

test_export_dimacs: tests/test_export_dimacs.c dimacs.o modelisation.o plateau.o logique.o commun.o listes.o
	$(CC) $(CFLAGS) -o $@ $^

test_sat_solver: tests/test_sat_solver.c sat_solver.o
	$(CC) $(CFLAGS) -o $@ $^
	
clean:
	rm -rf $(EXECUTABLES) *.o

fclean:
	rm -rf $(EXECUTABLES) *.o
	rm -rf *.dimacs
