CC=clang
CFLAGS=-Wall -Werror

EXECUTABLES = test_plateau test_listes
all: $(EXECUTABLES)

%.o: %.c %.h
	$(CC) -c $<

test_plateau: test_plateau.c plateau.o listes.o utiles.o
	$(CC) $(CFLAGS) -o $@ $^

test_listes: test_listes.c plateau.o listes.o utiles.o
	$(CC) $(CFLAGS) -o $@ $^

clean:
	rm -f $(EXECUTABLES) *.o *.gch
