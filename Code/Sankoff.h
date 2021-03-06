/**
	@file Sankoff.c 
	@brief Algoritmo di Sankoff
*/
typedef struct SankoffTree {
    int n_figli; // Numero figli
    int **score;
    struct SankoffTree *padre;
    struct SankoffTree *figli; // Punta al primo figlio
    struct SankoffTree *next;  // Punta al prossimo fratello
} SankoffTree;

extern void sankoff_algorithm(char*,char*);

