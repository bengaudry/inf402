CC=clang
CFLAGS=-Werror

EXECUTABLES = test_plateau test_listes main

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $<

main: main.c plateau.o listes.o utiles.o
	$(CC) $(CFLAGS) -o $@ $^

test_plateau: test_plateau.c plateau.o listes.o utiles.o
	$(CC) $(CFLAGS) -o $@ $^

test_listes: test_listes.c plateau.o listes.o utiles.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXECUTABLES) *.o
