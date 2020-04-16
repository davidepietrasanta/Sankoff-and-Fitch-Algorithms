#
# Makefile
#
start: Menu.o Parser.o Fitch.o Sankoff.o Code/Parser.h Code/Fitch.h Code/Sankoff.h
	gcc -o start menu.o Parser.o Fitch.o Sankoff.o
#
Menu.o: Code/menu.c
	gcc -c Code/menu.c
#
Parser.o: Code/Parser.c
	gcc -c Code/Parser.c
#
Fitch.o: Code/Fitch.c
	gcc -c Code/Fitch.c
#
Sankoff.o: Code/Sankoff.c
	gcc -c Code/Sankoff.c

