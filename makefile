#
# Makefile
#
start: Menu.o Parser.o Fitch.o Sankoff.o Fitch/Parser.h Sankoff/Parser.h Menu/Fitch.h Menu/Sankoff.h
	gcc -o start menu.o Parser.o Fitch.o Sankoff.o
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
Sankoff.o: Sankoff/Sankoff.c
	gcc -c Sankoff/Sankoff.c

