/**
	@file Fitch.h 
	@brief Algoritmo di Fitch
*/

typedef struct List {
	int num;
    int cont;
	struct List *next;
}List;


extern void fitch_algorithm(char*);


   