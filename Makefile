CC=clang
CFLAGS=-Werror

EXECUTABLES = test_plateau test_listes test_logique test_modelisation main

all: $(EXECUTABLES)

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

main: main.c plateau.o listes.o commun.o logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_plateau: tests/test_plateau.c plateau.o listes.o commun.o logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_listes: tests/test_listes.c plateau.o listes.o commun.o logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_logique: tests/test_logique.c logique.o
	$(CC) $(CFLAGS) -o $@ $^

test_modelisation: tests/test_modelisation.c modelisation.o plateau.o logique.o commun.o listes.o
	$(CC) $(CFLAGS) -o $@ $^


disp_all:
	make test_plateau
	clear
	./test_plateau plateaux/2x2.txt
	./test_plateau plateaux/3x3.txt
	./test_plateau plateaux/5x5.txt
	./test_plateau plateaux/6x6.txt

clean:
	rm -f $(EXECUTABLES) *.o
