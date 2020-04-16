

typedef struct Tree {
    int nF; //numero figli
	char* string;
    bool flag; //True if ok
    struct Tree* padre;
    struct Tree* figli; //punta al primo figlio
    struct Tree* next; //punta al prossimo fratello
}Tree;

extern int len(char*);

extern char* fstring(char*);
    
extern bool numericalString(char*);
    
extern Tree* doTree(char*);
    
extern void addNext(Tree*, Tree*);
   
extern char* reverse(char*);
   
extern int contSimbol(char*);
    
extern int* allSimbol(char*);
   
extern void setInit(char*, int);
   
extern char** extract(char*);
   
extern Tree* Newick(char*);
   
extern void printIntArray(int*, int);
    
extern void stampaMatrix(char**, int);
   
extern void printTree(Tree*);

extern int numeroNodi(Tree*);

extern int numeroCaratteri(char*);

extern int* statoMaxPerCarattere(char*);

extern char* treeToNewick(Tree*);
   