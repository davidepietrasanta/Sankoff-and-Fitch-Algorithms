#
# Makefile
#
all: Menu.o Parser.o Fitch.o Fitch/Parser.h Menu/Fitch.h
	gcc -o all menu.o Parser.o Fitch.o
#
Menu.o: Menu/menu.c
	gcc -c Menu/menu.c
#
Parser.o: Newick\ Parser/Parser.c
	gcc -c Newick\ Parser/Parser.c
#
Fitch.o: Fitch/Fitch.c
	gcc -c Fitch/Fitch.c
#

