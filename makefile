#
# Makefile somma
#
start: Parser.o Fitch.o Fitch/Parser.h
	gcc -o start Parser.o Fitch.o
#
Parser.o: Newick\ Parser/Parser.c
	gcc -c Newick\ Parser/Parser.c
#
Fitch.o: Fitch/Fitch.c
	gcc -c Fitch/Fitch.c
