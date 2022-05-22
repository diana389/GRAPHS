# Declaratiile de variabile
CC = gcc
CFLAGS = -g -Wall -lm -std=c11

# Regula de compilare
all: build

build: tema3.c
	$(CC) -o tema3 tema3.c $(CFLAGS)


# Regulile de "curatenie" (se folosesc pentru stergerea fisierelor intermediare\
si/sau rezultate)

clean:
	rm -f tema3
	rm -f *.o


