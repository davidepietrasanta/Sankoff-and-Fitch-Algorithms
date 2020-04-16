#include <math.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include "Parser.h"
#include <string.h>

/** 
 * Recupera la dimnesione di una lista
 */
#define size(arr) (sizeof(arr) / sizeof(arr[0]))

/**
	@file Sankoff.c 
	@brief Algoritmo di Sankoff
*/


/**
	Struttura per rappresentare un albero
    di array di liste con N figli
*/
typedef struct SankoffTree
{
    int nF; /**< Numero figli */
    int **score;
    struct SankoffTree *padre;
    struct SankoffTree *figli; /**< Punta al primo figlio */
    struct SankoffTree *next;  /**< Punta al prossimo fratello */
} SankoffTree;

const int inf = __INT_MAX__ / 16;

/**
 * Restituisce True se il char in ingresso è un numero, False altrimenti
 * @param character char in ingresso
 */
bool is_digit(char character)
{
    return (character >= 48 && character <= 57);
}

/**
 * Restituisce il numero rappresentato dal char in ingresso
 * @param c char in ingresso
 */
int digit_to_num(char c)
{
    return c - '0';
}

/**
 * Restituisce True se il carattere in input corrisponde al numero
 * @param c char in ingresso
 * @param num numero in ingreesso
 */
bool same_digit(char c, int num)
{
    return (num == digit_to_num(c));
}

/**
 *  Crea un albero con la stessa morfologia di quello in input e che ne
 *  inizializza i costi delle foglie per ogni carattere
 * @param tree albero in formato Tree
 * @param n_states numero totali di differenti stati
 * @param n_characters numero di caratteri
 */
SankoffTree *mk_sankoff_tree(Tree *tree, int n_states, int n_characters)
{
    SankoffTree *s_tree = (SankoffTree *)malloc(sizeof(SankoffTree));
    s_tree->nF = tree->nF;

    /* Se il nodo originale non ha informazioni allora neanche il nodo del
       nuovo albero ne avrà, altrimenti per ogni carattere si imposterà a 0 il
       costo per lo stato che assume il carattere in quel nodo e +infinito
       per tutti gli altri stati */
    if (!is_digit(tree->string[0]))
        s_tree->score = NULL;
    else
    {
        int **score = (int **)malloc(n_characters * sizeof(int *));
        for (int n_char = 0; n_char < n_characters; n_char++)
        {
            char character = tree->string[n_char];
            score[n_char] = (int *)malloc(n_states * sizeof(int));

            for (int n_state = 0; n_state < n_states; n_state++)
                score[n_char][n_state] = 
                    same_digit(character, n_state) ? 0 : inf;
        }
        s_tree->score = score;
    }
    s_tree->padre = NULL;

    // Se il nodo ha almeno un figlio si costruiscono anche i nodi dei figli
    if (s_tree->nF > 0)
    {
        // stSon sarà il primo figlio
        Tree *tree_son = tree->figli;
        SankoffTree *st_son =
            mk_sankoff_tree(tree_son, n_states, n_characters);
        st_son->padre = s_tree;
        s_tree->figli = st_son;

        /* Fino a quando non si creano tutti i figli si crea un nuovo nodo e
           viene inizializzato sulla base di quello equivalente 
           nell'albero originale */
        for (int n_son = 1; n_son < tree->nF; n_son++)
        {
            Tree *next = tree_son->next;
            SankoffTree *tmp_son =
                mk_sankoff_tree(next, n_states, n_characters);
            tmp_son->padre = s_tree;
            st_son->next = tmp_son;
            st_son = tmp_son;
            tree_son = next;
        }
    }
    else
        s_tree->figli = NULL;
    s_tree->next = NULL;
}

/**
 *  Effettua il calcolo del parsimony score dati i figli
 * @param nodes nodo iniziale su cui calcolare parsimony score
 * @param n_states numero massimo di stati differenti
 * @param n_characters numero di caratteri
 * @param cost matrice dei costi
 */
int **join(SankoffTree *nodes, int n_states, int n_characters, int **cost)
{
    int **joined = (int **)malloc(n_characters * sizeof(int *));
    for (int n_char = 0; n_char < n_characters; n_char++)
    {
        joined[n_char] = (int *)malloc(n_states * sizeof(int));
        int num_brothers = nodes->padre->nF;
        for (int i = 0; i < n_states; i++)
        {
            joined[n_char][i] = 0;
            int mins[num_brothers];

            for (int n_bro = 0; n_bro < num_brothers; n_bro++)
                mins[n_bro] = inf;

            for (int j = 0; j < n_states; j++)
            {
                if (mins[0] > cost[i][j] + nodes->score[n_char][j])
                    mins[0] = cost[i][j] + nodes->score[n_char][j];

                SankoffTree *next = nodes->next;
                for (int n_bro = 1; n_bro < num_brothers; n_bro++)
                {
                    if (mins[n_bro] > cost[i][j] + next->score[n_char][j])
                        mins[n_bro] = cost[i][j] + next->score[n_char][j];
                    next = next->next;
                }
            }
            for (int n_bro = 0; n_bro < num_brothers; n_bro++)
                joined[n_char][i] = joined[n_char][i] + mins[n_bro];
        }
    }
    return joined;
}

/**
 *  Per ogni nodo se non ha lo score lo calcola sulla base di quello
 *  dei figli (procedura ricorsiva)
 * @param node radice a partire da cui calcolare sankoff
 * @param n_states numero totale di stati diversi
 * @param n_characters numero di caratteri
 * @param cost matrice dei costi
 */
void *Sankoff(SankoffTree *node, int n_states, int n_characters, int **cost)
{
    if (node->score == NULL)
    {
        SankoffTree *node_son = node->figli;
        Sankoff(node_son, n_states, n_characters, cost);
        for (int n_son = 1; n_son < node->nF; n_son++)
        {
            SankoffTree *tmp_son = node_son->next;
            Sankoff(tmp_son, n_states, n_characters, cost);
            node_son = tmp_son;
        }
        node->score = join(node->figli, n_states, n_characters, cost);
    }
}

/**
 *  Ricava il numero rappresentato da una sequenza di caratteri (cifre) 
 * @param mat stringa rappresentante la matrice dei costi
 * @param i posizione di inizio da cui ricavare il numero
 * @param digits_number numero di cifre da ricavare
 */
int get_num_from_string(char *mat, int i, int digits_number)
{
    int pot = 1, number = 0;
    for (int j = 0; j < digits_number; j++)
    {
        number = number + digit_to_num(mat[i - 1 - j]) * pot;
        pot = pot * 10;
    }
    return number;
}

/**
 *  Ricava la matrice dei costi 
 * @param mat stringa rappresentante la matrice dei costi
 * @param n_states restituirà il numero massimo di stati
 */
int **get_matrix(char *mat, int *n_states)
{
    int length_of_file = strlen(mat);
    int i = 0;
    while (mat[i] != '\n')
        i++;
    *n_states = get_num_from_string(mat, i, i);
    i++;
    int **matrix = malloc(sizeof(int *) * (*n_states));
    int *row = malloc(sizeof(int) * (*n_states));
    int col_index = 0, row_index = 0, digits_number = 0;
    for (i; i < length_of_file; i++)
    {
        if (mat[i] == '\n')
        {
            row[col_index] = get_num_from_string(mat, i, digits_number);
            digits_number = 0;
            matrix[row_index] = row;
            row_index++;
            col_index = 0;
            row = malloc(sizeof(int) * (*n_states));
        }
        else if (mat[i] == ' ')
        {
            row[col_index] = get_num_from_string(mat, i, digits_number);
            col_index++;
            digits_number = 0;
        }
        else
            digits_number++;
    }
    return matrix;
}

/** 
 * Salva l'output della elaborazione
 * @param path percorso del file
 * @param string contenuto del file da salvare
 */
bool save(char *path, char *string)
{
    /* Salva la stringa in un file simile a quello inserito */
    int len_path = len(path);
    char *new_path = (char *)malloc((len_path + 5) * (sizeof(char)));
    strcpy(new_path, path);

    // Rimuovere la dicitura .txt finale
    new_path[len_path - 1] = '\0';
    new_path[len_path - 2] = '\0';
    new_path[len_path - 3] = '\0';
    new_path[len_path - 4] = '\0';
    strcat(new_path, "Sankoff.txt");

    FILE *file;
    file = fopen(new_path, "w");
    if (file == NULL)
    {
        perror("Errore in apertura del file");
        return false;
    }

    fprintf(file, "%s\n", string);
    fclose(file);
    return true;
}

/**
 * Produce un albero con i relativi stati di ogni carattere a partire
 * dal loro parsimony score
 * @param tree tree con topologia corretta (uguale a quella di s_tree)
 * @param s_tree sankoff tree
 * @param n_s numero di stati
 * @param n_c numero di caratteri
 * @param c matrice dei costi
 */
void st_to_tree(Tree *tree, SankoffTree *s_tree, int n_s, int n_c, int **c)
{
    if (s_tree != NULL)
    {
        if (s_tree->padre == NULL)
        {
            int *mins = malloc(sizeof(int) * n_c);
            for (int n_char = 0; n_char < n_c; n_char++)
            {
                mins[n_char] = 0;
                for (int n_state = 1; n_state < n_s; n_state++)
                {
                    int min = s_tree->score[n_char][mins[n_char]];
                    if (s_tree->score[n_char][n_state] < min)
                        mins[n_char] = n_state;
                }
            }
            char *stringa = malloc((n_c + 1) * sizeof(char));
            for (int n_char = 0; n_char < n_c; n_char++)
                stringa[n_char] = mins[n_char] + '0';
            stringa[n_c] = '\0';
            tree->string = stringa;
        }
        else
        {

            int *mins = malloc(sizeof(int) * n_c);
            for (int n_char = 0; n_char < n_c; n_char++)
            {
                mins[n_char] = 0;
                for (int state = 1; state < n_s; state++)
                {
                    //sp = stato del padre
                    int sp = digit_to_num(tree->padre->string[n_char]);
                    //sS = min score del carattere fino a iterazione corrente
                    int sS = s_tree->score[n_char][mins[n_char]];
                    int min = sS + c[mins[n_char]][sp];
                    if (s_tree->score[n_char][state] + c[state][sp] < min)
                        mins[n_char] = state;
                }
            }
            char *stringa = malloc((n_c + 1) * sizeof(char));
            for (int n_char = 0; n_char < n_c; n_char++)
                stringa[n_char] = mins[n_char] + '0';
            stringa[n_c] = '\0';
            tree->string = stringa;
        }

        if (s_tree->nF > 0)
        {
            Tree *son = tree->figli;
            SankoffTree *s_son = s_tree->figli;

            st_to_tree(son, s_son, n_s, n_c, c);

            for (int n_son = 1; n_son < s_tree->nF; n_son++)
            {
                s_son = s_son->next;
                son = son->next;
                st_to_tree(son, s_son, n_s, n_c, c);
            }
        }
    }
}

/**
 * Esegue sankoff sull'albero e la matrice dei costi presenti nel file
 * in input
 * @param string_path path dove è presente l'albero
 * @param mat_path path dove è presente la matrice dei costi
 */
void sankoff_algorithm(char *string_path, char *mat_path)
{
    char *string = fstring(string_path);
    char *matrix = fstring(mat_path);
    int n_states = 0;
    int **cost = get_matrix(matrix, &n_states);
    int n_characters = numeroCaratteri(string);
    printf("Input Tree: %s\n", string);
    Tree *tree = Newick(string);
    SankoffTree *stree = mk_sankoff_tree(tree, n_states, n_characters);
    Sankoff(stree, n_states, n_characters, cost);
    st_to_tree(tree, stree, n_states, n_characters, cost);
    char *stringa_newick = treeToNewick(tree);
    printf("Cost matrix: \n\nNumber of states: ");
    printf("%s", matrix);
    printf("\nOutput Sankoff Tree: %s\n", stringa_newick);
    save(string_path, stringa_newick);
}