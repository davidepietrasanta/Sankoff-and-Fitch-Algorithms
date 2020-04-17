#ifndef PARSER_H
#define PARSER_H

/**
	@file Fitch.h 
	@brief Algoritmo di Fitch
*/

typedef struct Tree {
    int n_figli; //numero figli
	char* string;
    bool flag; //True if ok
    struct Tree* padre;
    struct Tree* figli; //punta al primo figlio
    struct Tree* next; //punta al prossimo fratello
}Tree;

extern int len(char*);

extern char* fstring(char*);
    
extern bool numerical_string(char*);
    
extern Tree* do_tree(char*);
    
extern void add_next(Tree*, Tree*);
   
extern char* reverse(char*);
   
extern int cont_simbol(char*);
    
extern int* all_simbo(char*);
   
extern void set_init(char*, int);
   
extern char** extract(char*);
   
extern Tree* Newick(char*);
   
extern void print_int_array(int*, int);
    
extern void stampa_matrix(char**, int);
   
extern void print_tree(Tree*);

extern int numero_nodi(Tree*);

extern int numero_caratteri(char*);

extern int* stato_max_per_carattere(char*);

extern char* tree_to_Newick(Tree*);

#endif