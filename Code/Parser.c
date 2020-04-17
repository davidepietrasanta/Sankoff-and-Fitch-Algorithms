 #include <stdlib.h>
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scan_figli. 
#include <string.h> //Per manipolare le stringhe. 

/**
	@file Parser.c
	@brief Parser per la conversione di un Newick Tree
*/

/**
	Struttura per rappresentare un albero
    di stringhe con N figli
*/
typedef struct Tree {
    int n_figli; //numero figli
	char* string;
    bool flag; //True if ok
    struct Tree* padre;
    struct Tree* figli; //punta al primo figlio
    struct Tree* next; //punta al prossimo fratello
}Tree;

/**
	Restituisce la lunghezza della stringa
    @param s stringa
*/
int len(char *s){

    int i;
    for(i = 0; *(s+i) != '\0' ; i++)
        ;
    return i;
} 

/**
	Legge un file e ritorna una copia 
    restituendo una stringa
    @param path Percorso del file
*/
char* fstring(char *path){

    char* buffer = 0;
    long len;
    FILE * file = fopen (path, "r"); 

    if (file){
        fseek(file, 0, SEEK_END);
        len = ftell(file);
        fseek(file, 0, SEEK_SET);
        buffer = (char*)malloc((len+1)*sizeof(char));
        if (buffer){
            fread(buffer, sizeof(char), len, file);
        }
        fclose (file);
    }
    buffer[len] = '\0';
    return buffer;
}

/**
	True se la stringa e' numerica, 
    False altrimenti
    @param s stringa
*/
bool numerical_string(char *s){
    int i;
    if( len(s) == 0 ){
        return false;
    }

    for(i = 0; *(s+i) != '\0' ; i++){
        if( s[i] < '0' || s[i] > '9' ){
            return false;
        }
    }
    return true;
}

/**
	Ritorna un Tree con il valore
    della stringa c
    @param c stringa 
*/
Tree* do_tree(char* c){

    Tree *l = (Tree*)malloc(sizeof(Tree));
    l->n_figli = 0;
    l->string = c;
    l->flag = numerical_string(c);
    l->padre = NULL;
    l->figli = NULL;
    l->next = NULL;

    return l;
}

/**
	Aggiunge un fratello, come next di t, 
    in caso ci sia gia' un fratello va in fondo 
    e aggiunge il fratello
    @param t Albero in cui si vuole inserire 
    @param brother Albero che si vuole inserire come fratello
*/
void add_next(Tree* t, Tree* brother){

    if( brother == NULL ){
        return;
    }
    if( t == NULL ){
        t = brother;
        return;
    }
    brother->padre = t->padre;
    if( t->padre != NULL ){
        brother->padre->n_figli = brother->padre->n_figli + 1;
    }
    while(t->next != NULL){
        t = t->next;
    }
    t->next = brother;
}

/**
	Restituisce la strina inversa 
    @param s stringa
*/
char* reverse(char *s){

    int lens = len(s);
    int i;
    char *temp = (char*)malloc((lens+1)*sizeof(char));
    for(i = 0; i < lens; i++){
        if( s[lens-1-i] == ')' || s[lens-1-i] == '(' ){
            if( s[lens-1-i] == ')' ){
                temp[i] = '(';
            }
            else{
                temp[i] = ')';
            }
        }
        else{
            temp[i] = s[lens-1-i];
        }
    }
    temp[lens] = '\0';
    return temp;
}

/**
	Conta i simboi nella stringa.
    Come simboli di intende:
    '(' , ')' oppure ','
    @param s stringa
*/
int cont_simbol(char *s){

    int i;
    int cont=0;
    int lens = len(s);
     for(i = 0; i < lens; i++){
        if( s[i] == ')' || s[i] == '(' || s[i] == ',' ){
            cont++;
        }
    }
    return cont;
}

/**
	Ritorna un array di int contenente
    la lista degli indici in cui sono 
    presente dei simboli (rilevati da 
    cont_simbol) 
    @param s stringa
*/
int* all_simbol(char *s){

    int cont = cont_simbol(s);
    int cont_temp = 0;
    int i;
    int *array = (int*)malloc((cont)*sizeof(int));
    for(i = 0; s[i] != '\0' ; i++){
        if( s[i] == ')' || s[i] == '(' || s[i] == ',' ){
            array[cont_temp] = i;
            cont_temp++;
        }
    }
    return array;
}

/**
	Setta la stringa s di lunghezza i
    (oppure fino ad i)
    @param s stringa
    @param i lunghezza dela stringa
*/
void set_init(char *s, int i){

    int j;
    for(j = 0; j < i; j++){
        s[j] = '\0';
    }
}

/**
	Stampa un array di int di lunghezza len
    (oppure fino a len)
    @param array Array di int da stampare
    @param len Lunghezza dell'array
*/
void print_int_array(int *array, int len){

    int i;
    for(i = 0; i < len-1; i++){
        printf("%d, ",array[i]);
    }
    printf("%d \n",array[len-1]);
}

/**
	Ritorna un array di stringhe contente tutto cio'
    che e' compreso tra due simboli 
    @param rev stringa invertita
*/
char** extract(char *rev){

    int i;
    int cont = cont_simbol(rev);
    int *array = all_simbol(rev); //lunghezza cont
    char **estratti = (char**)malloc( cont * sizeof(char*));
    for(i =0; i < cont; i++){
        estratti[i] = (char*)malloc( len(rev) * sizeof(char)); //just to be sure...
        set_init(estratti[i], len(rev));
    }
    
    strncpy(estratti[0], rev, array[0]);
    for(i = 1; i < cont-1; i++){
        strncpy(estratti[i], rev+array[i-1]+1, array[i]-array[i-1]-1);
    }
    strncpy(estratti[i], rev+array[cont-2]+1, array[cont-1]-array[cont-2]-1);

    return estratti;
}

/**
	Stampa una matrice quadrata di char 
    di dimensione x
    @param s Matrice quadrata di char
    @param x Dimensione della matrice
*/
void stampa_matrix(char **s, int x){

    int i;
    for(i = 0; i < x-1; i++){
        printf("%s, ",s[i]);
    }
    printf("%s\n", s[x-1]);
}

/**
	Ritorna l'albero relativo ala stringa
    secondo la codifca di Newick 
    @param string stringa
*/
Tree* Newick(char* string){

    Tree* root = (Tree*)malloc(sizeof(Tree));
    char *rev = reverse(string);
    int cont = cont_simbol(rev);
    char **estratto = extract(rev);

    int *index_simbols = all_simbol(rev); //lunghezza cont
    int i; 
    root = do_tree(estratto[0]);

    for(i = 0; i < cont-1; i++){
        if( rev[index_simbols[i]] == '(' ){
            if( root->figli == NULL ){
                root->figli = do_tree(estratto[i+1]);
                root->n_figli = root->n_figli + 1;
                root->figli->padre = root;
                root = root->figli;
            }
            else{
                root = root->figli;
                add_next(root,do_tree(estratto[i+1]));
            }
        }
        if( rev[index_simbols[i]] == ',' ){
            add_next(root, do_tree(estratto[i+1]));
            if( root->next != NULL){
                root = root->next;
            }
        }
        if( rev[index_simbols[i]] == ')' ){
            root = root->padre;
        }
    }
    return root->padre;
}

/**
	Stampa un Tree 
    @param t Albero Tree
*/
void print_tree(Tree* t){

    if( t == NULL ){
        printf("Empty \n");
    }

    printf("%s \n", t->string);
    if( t->figli != NULL){
        print_tree(t->figli);
    }
    if( t->next != NULL){
        print_tree(t->next);
    }
    
}

/**
	Conta il numero di nodi dell'albero 
    @param t Albero Tree
*/
int numero_nodi(Tree *t){
    if( t == NULL ){
        return 0;
    }

    int cont = 1;
    if( t->figli != NULL){
        cont = cont + numero_nodi(t->figli);
    }
    if( t->next != NULL){
        cont = cont + numero_nodi(t->next);
    }
    return cont;
}

/**
	Conta il numero di Caratteri delle specie
    @param string stringa
*/
int numero_caratteri(char *string){
    
    char ** estratti = extract(string);
    int n_simboli = cont_simbol(string);
    int max = 0;
    if(string == NULL ){
        return 0;
    }
    int i;
    for(i = 0; i < n_simboli; i++){
        if( numerical_string(estratti[i]) && max < len(estratti[i]) ){
            max = len(estratti[i]);
        }
    }
    return max;
}

/**
	Ritorna un vettore con lo stato massimo di ogni
    carattere delle specie 
    @param string stringa
*/
int* stato_max_per_carattere(char *string){
    if(string == NULL ){
        return 0;
    }

    char ** estratti = extract(string);
    int n_simboli = cont_simbol(string);
    int n_caratteri = numero_caratteri(string);
    int *max = (int*)malloc(n_caratteri*sizeof(int)); 
    int i;
    int j;
    for(i = 0; i < n_caratteri; i++){
        max[i] = 0;
    }

    for(i = 0; i < n_simboli; i++){
        if( numerical_string(estratti[i]) ){
            for(j = 0; j < n_caratteri; j++){
                if( max[j] < estratti[i][j] - '0' ){
                    max[j] = estratti[i][j] - '0';
                }
            }   
        }
    }
    

    //stampa_matrix(estratti, n_simboli);
    //printf("estratti[0] %s.\n", estratti[0]);

    //printf("max ");
    //print_int_array(max, n_caratteri);
    //printf("fine\n");
    return max;
}

/**
	Ritorna la rappresentazione Newick 
    dell'albero in input
    @param t Albero Tree
*/
char* tree_to_Newick(Tree *t){

    if( t == NULL ){
        return NULL;
    }

    char *res = t->string;
    if( t->figli != NULL){
        res = strcat(res, "(");
        res = strcat(res, reverse(tree_to_Newick(t->figli)));
        res = strcat(res, ")");
        
    }
    if( t->next != NULL){
        res = strcat(res, ",");
        res = strcat(res, reverse(tree_to_Newick(t->next)));
    }
    res = reverse(res);
    return res;
}
