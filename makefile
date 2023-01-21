all: ponto

ponto: ponto.o lib.o
	gcc -o ponto ponto.o lib.o 

ponto.o: ponto.c
	gcc -c ponto.c

lib.o: lib.c
	gcc -c lib.c 
