.PHONY: all
all : TICO

.PHONY : clean
clean:
	rm -rf TICO.o
	rm -rf TICO

all: TICO

TICO : TICO.o
	gcc -o TICO TICO.o

TICO.o : TICO.c
	gcc -g -c TICO.c -o TICO.o
