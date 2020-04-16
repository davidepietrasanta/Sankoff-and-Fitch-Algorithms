typedef struct SankoffTree {
    int nF; // Numero figli
    int **score;
    struct SankoffTree *padre;
    struct SankoffTree *figli; // Punta al primo figlio
    struct SankoffTree *next;  // Punta al prossimo fratello
} SankoffTree;

extern void sankoff_algorithm(char*,char*);

