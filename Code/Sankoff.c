#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "Parser.h"
#include <string.h>

#define size(arr) (sizeof(arr) / sizeof(arr[0]))

typedef struct SankoffTree
{
    int nF; // Numero figli
    int **score;
    struct SankoffTree *padre;
    struct SankoffTree *figli; // Punta al primo figlio
    struct SankoffTree *next;  // Punta al prossimo fratello
} SankoffTree;

const int inf = __INT_MAX__ / 16;

bool isDigit(char character)
{
    return (character >= 48 && character <= 57);
}

bool sameDigit(char c, int num)
{
    return (num == c - '0');
}

/* Crea un albero con la stessa morfologia di quello in input e che ne
   inizializza i costi delle foglie per ogni carattere */
SankoffTree *makeSankoffTree(Tree *tree, int nStates, int nCharacters)
{
    SankoffTree *sTree = (SankoffTree *)malloc(sizeof(SankoffTree));
    sTree->nF = tree->nF;

    /* Se il nodo originale non ha informazioni allora neanche il nodo del
       nuovo albero ne avrà, altrimenti per ogni carattere si imposterà a 0 il
       costo per lo stato che assume il carattere in quel nodo e +infinito
       per tutti gli altri stati */
    if (!isDigit(tree->string[0]))
        sTree->score = NULL;
    else
    {
        int **score = (int **)malloc(nCharacters * sizeof(int *));
        for (int nChar = 0; nChar < nCharacters; nChar++)
        {
            char character = tree->string[nChar];
            score[nChar] = (int *)malloc(nStates * sizeof(int));

            for (int nState = 0; nState < nStates; nState++)
                score[nChar][nState] = sameDigit(character, nState) ? 0 : inf;
        }
        sTree->score = score;
    }
    sTree->padre = NULL;

    // Se il nodo ha almeno un figlio si costruiscono anche i nodi dei figli
    if (sTree->nF > 0)
    {
        // sankoffTreeSon sarà il primo figlio
        Tree *treeSon = tree->figli;
        SankoffTree *stSon = makeSankoffTree(treeSon, nStates, nCharacters);
        stSon->padre = sTree;
        sTree->figli = stSon;

        /* Fino a quando non si creano tutti i figli si crea un nuovo nodo e
           viene inizializzato sulla base di quello equivalente 
           nell'albero originale */
        for (int nSon = 1; nSon < tree->nF; nSon++)
        {
            Tree *next = treeSon->next;
            SankoffTree *tmpSon = makeSankoffTree(next, nStates, nCharacters);
            tmpSon->padre = sTree;
            stSon->next = tmpSon;
            stSon = tmpSon;
            treeSon = next;
        }
    }
    else
        sTree->figli = NULL;
    sTree->next = NULL;
}

// Effettua il calcolo del parsimony score dati i figli
int **join(SankoffTree *nodes, int nStates, int nCharacters, int **cost)
{
    int **joined = (int **)malloc(nCharacters * sizeof(int *));
    for (int nChar = 0; nChar < nCharacters; nChar++)
    {
        joined[nChar] = (int *)malloc(nStates * sizeof(int));
        int numBrothers = nodes->padre->nF;
        for (int i = 0; i < nStates; i++)
        {
            joined[nChar][i] = 0;
            int mins[numBrothers];

            for (int nBro = 0; nBro < numBrothers; nBro++)
                mins[nBro] = inf;

            for (int j = 0; j < nStates; j++)
            {
                if (mins[0] > cost[i][j] + nodes->score[nChar][j])
                    mins[0] = cost[i][j] + nodes->score[nChar][j];

                SankoffTree *next = nodes->next;
                for (int nBro = 1; nBro < numBrothers; nBro++)
                {
                    if (mins[nBro] > cost[i][j] + next->score[nChar][j])
                        mins[nBro] = cost[i][j] + next->score[nChar][j];
                    next = next->next;
                }
            }
            for (int nBro = 0; nBro < numBrothers; nBro++)
                joined[nChar][i] = joined[nChar][i] + mins[nBro];
        }
    }
    return joined;
}

/* Per ogni nodo se non ha lo score lo calcola sulla base di quello 
   dei figli (procedura ricorsiva) */
void *Sankoff(SankoffTree *node, int nStates, int nCharacters, int **cost)
{
    if (node->score == NULL)
    {
        SankoffTree *nodeSon = node->figli;
        Sankoff(nodeSon, nStates, nCharacters, cost);
        for (int nSon = 1; nSon < node->nF; nSon++)
        {
            SankoffTree *tmpSon = nodeSon->next;
            Sankoff(tmpSon, nStates, nCharacters, cost);
            nodeSon = tmpSon;
        }
        node->score = join(node->figli, nStates, nCharacters, cost);
    }
}

/* Produce un albero con i relativi stati di ogni carattere a partire
   dal loro parsimony score */
Tree *sankoffTreeToTree(SankoffTree *sTree, int nStates, int nCharacters)
{
    Tree *tree = (Tree *)malloc(sizeof(Tree));
    tree->nF = sTree->nF;
    if (sTree->score == NULL)
        tree->string = NULL;
    else
    {
        int *mins = malloc(sizeof(int) * nCharacters);
        for (int nChar = 0; nChar < nCharacters; nChar++)
        {
            mins[nChar] = 0;
            for (int nState = 1; nState < nStates; nState++)
            {
                int min = sTree->score[nChar][mins[nChar]];
                if (sTree->score[nChar][nState] < min)
                    mins[nChar] = nState;
            }
        }
        char *stringa = malloc((nCharacters + 1) * sizeof(char));
        for (int nChar = 0; nChar < nCharacters; nChar++)
            stringa[nChar] = mins[nChar] + '0';
        stringa[nCharacters] = '\0';
        tree->string = stringa;
    }
    tree->padre = NULL;
    // Se il nodo ha almeno un figlio si costruiscono anche i nodi dei figli
    if (tree->nF > 0)
    {
        // tSon sarà il primo figlio
        Tree *tSon = sankoffTreeToTree((sTree->figli), nStates, nCharacters);
        tSon->padre = tree;
        tree->figli = tSon;

        /* Fino a quando non si creano tutti i figli si crea un nuovo nodo e
           viene inizializzato sulla base di quello 
           equivalente nell'albero originale */
        SankoffTree *stSon = sTree->figli;
        for (int i = 1; i < sTree->nF; i++)
        {
            SankoffTree *next = stSon->next;
            Tree *tmpSon = sankoffTreeToTree(next, nStates, nCharacters);
            tmpSon->padre = tree;
            tSon->next = tmpSon;
            tSon = tmpSon;
            stSon = next;
        }
    }
    else
        tree->figli = NULL;
    tree->next = NULL;
    return tree;
}

/* Ricava il numero di caratteri a partire da un albero */
int getNumOfCharacters(Tree *tree)
{
    if (tree->string == NULL)
        return -1;
    if (isDigit(tree->string[0]))
        return strlen(tree->string);
    if (tree->nF == 0)
        return -1;
    else
    {
        Tree *son = tree->figli;
        int n = getNumOfCharacters(son);
        if (n = ! - 1)
            return n;

        for (int i = 1; i < tree->nF; i++)
        {
            n = getNumOfCharacters(son->next);
            if (n != -1)
                return n;
            son = son->next;
        }
    }
}

/* Ricava il numero rappresentato da una sequenza di caratteri (cifre) */
int getNumFromString(char *mat, int i, int digitsNumber)
{
    int pot = 1, number = 0;
    for (int j = 0; j < digitsNumber; j++)
    {
        number = number + (mat[i - 1 - j] - '0') * pot;
        pot = pot * 10;
    }
    return number;
}

/* Ricava la matrice dei costi */
int **getMatrix(char *mat, int *nStates)
{
    int lengthOfFile = strlen(mat);
    int i = 0;
    while (mat[i] != '\n')
        i++;
    *nStates = getNumFromString(mat, i, i);
    i++;
    int **matrix = malloc(sizeof(int *) * (*nStates));
    int *row = malloc(sizeof(int) * (*nStates));
    int colIndex = 0, rowIndex = 0, digitsNumber = 0;
    for (i; i < lengthOfFile; i++)
    {
        if (mat[i] == '\n')
        {
            row[colIndex] = getNumFromString(mat, i, digitsNumber);
            digitsNumber = 0;
            matrix[rowIndex] = row;
            rowIndex++;
            colIndex = 0;
            row = malloc(sizeof(int) * (*nStates));
        }
        else if (mat[i] == ' ')
        {
            row[colIndex] = getNumFromString(mat, i, digitsNumber);
            colIndex++;
            digitsNumber = 0;
        }
        else
            digitsNumber++;
    }
    return matrix;
}

void sankoff_algorithm(char *stringP, char *matP)
{
    char *string = fstring(stringP);
    char *matrix = fstring(matP);
    int nStates = 0;
    int **cost = getMatrix(matrix, &nStates);
    printf("Input Tree: %s\n", string);
    Tree *tree = Newick(string);
    int nCharacters = getNumOfCharacters(tree);
    SankoffTree *stree = makeSankoffTree(tree, nStates, nCharacters);
    Sankoff(stree, nStates, nCharacters, cost);
    tree = sankoffTreeToTree(stree, nStates, nCharacters);
    char *stringaNewick = treeToNewick(tree);
    printf("Output Sankoff Tree: %s\n", stringaNewick);
}