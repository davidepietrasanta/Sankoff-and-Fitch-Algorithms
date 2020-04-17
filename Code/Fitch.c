
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C.
#include <string.h> //Per manipolare le stringhe. 
#include <stdlib.h>
#include "Parser.h"

/**
	@file Fitch.c 
	@brief Algoritmo di Fitch
*/

/**
	Struttura per rappresentare una lista
    linkata semplice con un valore(num)
    e un peso(cont)
*/
typedef struct List {
	int num; //valore
    int cont; //peso
	struct List *next;
}List;

/**
	Struttura per rappresentare un albero
    di array di liste con N figli
*/
typedef struct FitchTree {
    int n_figli; //numero figli
	List **lista;
    struct FitchTree* padre;
    struct FitchTree* figli; //punta al primo figlio
    struct FitchTree* next; //punta al prossimo fratello
}FitchTree;

/**
    Ritorna una lista con una cella sola e valore i
    @param i valore della lista creata
*/
List* do_list(int i) { 

    List *l = (List*)malloc(sizeof(List));
    l->num = i;
    l->cont = 1;
    l->next = NULL;

    return l;
}

/**
    Ritorna la lunghezza dela lista
    @param l lista 
*/
int list_len(List *l) {

    if (l == NULL) {
        return 0;
    }
	int i=1;
    List *new = l;
    while (new->next != NULL) {
        new = new->next;
		i++;
    }
    return i;
}

/**
    Ritorna l'ultimo elemento (cella) della lista
    @param l lista
*/
List* last(List* l) { 

    if (l == NULL) {
        return NULL;
    }
    List *new = l;
    while (new->next != NULL) {
        new = new->next;
    }
    return new;
}

/**
    Aggiunge un elemento di valore i in coda alla lista
    @param l lista a cui si vuole aggiungere un elemento
    @param i valore dell'elemento che si vuole aggiungere
*/
void add_list(List *l, int i) {
    List *new = (List*)malloc(sizeof(List)); 
    List* ultimo = last(l); 
    new->next = NULL; 
    new->num = i;
    new->cont = 1;
	ultimo->next = new;
}

/**
    Aggiunge un elemento di valore i e con peso c in coda alla lista
    @param l lista a cui si vuole aggiungere un elemento
    @param i valore dell'elemento che si vuole aggiungere
    @param c peso (cont) dell'elemento che si vuole inserire
*/
void add_list_cont(List *l, int i, int c){ 
    List *new = (List*)malloc(sizeof(List)); 
    List* ultimo = last(l); 
    new->next = NULL; 
    new->num = i;
    new->cont = c;
	ultimo->next = new;
}

/**
    Converte un char numerico tra '0' e '9' (inclusi)
    in un numero equivalente 
    @param c char in ['0', '9'] 
*/
int my_atoi(char c){
    int num = c - '0';
    return num;
}

/**
    Ritorna l'equivalente List della stringa inserita
    @param string stringa da convertire
*/
List* string_to_list(char *string){

    int len_s = len(string);
    if( len_s < 1 ){
        return NULL;
    }
    List *temp; 
    List *new = do_list(my_atoi(string[0]));
    List *act = new;
    int i;
    for(i = 1; i < len_s; i++){
        temp = do_list(my_atoi(string[i]));
        act->next = temp;
        act = act-> next;
    }
    return new;
}

/**
    Converte una stringa in un array di liste di
    lunghezza uno
    @param string stringa che si vuole convertire
    @param array_list array di liste equivalente a string
*/
void string_to_array_list(char* string, List* array_list[]){

    int len_s = len(string);
    if( len_s < 1 ){
        array_list = NULL;
    }
    int i;
    
    for(i = 0; i < len_s; i++){
        array_list[i] = do_list(my_atoi(string[i]));
    }
}

/**
    Converta un array di liste di lunghezza uno in una stringa
    ATTENZIONE la lunghezza di array_list deve esser len_s
    @param array_list array di liste che si vuole convertire
    @param len_s lunghezza dell'array di liste
*/
char* array_list_to_string(List* array_list[], int len_s){
    
    int i;
    char *string = (char*)malloc(sizeof(char)*(len_s+1));
    string[len_s] = '\0';
    for(i = 0; i < len_s; i++){
        string[i] = array_list[i]->num + '0';
    }
    return string;
}

/**
    Copia a in b, assumendoli di lunghezza uguale
    @param b array di liste in cui avviene la copia
    @param a array di liste che si copia
    @param lens lunghezza degli array di liste
*/
void array_list_copy(List* b[], List* a[], int lens){
    int i;
    for(i = 0; i < lens; i++){
        b[i] = do_list(a[i]->num);
    }
}

/**
    Stampa una List
    @param l lista
*/
void stampa_list(List *l){

    if (l == NULL) {
        printf("empty \n");
        return;
    }
	int i=1;
    List *new = l;
    while (new != NULL) {
        printf("%d ", new->num);
        new = new->next;
		i++;
    }
    printf("\n");
}

/**
    Stampa una List mostrando anche il peso (cont)
    @param l lista
*/
void stampa_list_dettagliata(List *l){

    if (l == NULL) {
        printf("empty \n");
        return;
    }
	int i=1;
    List *new = l;
    while (new != NULL) {
        printf("( %d, c: %d)", new->num, new->cont);
        new = new->next;
		i++;
    }
    printf("\n");
}

/**
    Stampa un' array di List di lunghezza lens
    (oppure fino a lens)
    @param L array di List
    @param lens lunghezza dell'array dell'array di List
*/
void stampa_array_list(List* L[], int lens) {

    if (L == NULL) {
        printf("empty \n");
        return;
    }

    int i;
	for(i = 0; i < lens; i++){
        stampa_list(L[i]);
    }
}

/**
    Stampa un' array di List di lunghezza lens
    mostrando anche il peso (cont), se la 
    lunghezza dell'array e' maggiore di lens
    stampera' fino a lens
    @param L array di List
    @param lens lunghezza del'array di List
*/
void stampa_array_list_dettagliata(List* L[], int lens) {

    if (L == NULL) {
        printf("empty \n");
        return;
    }

    int i;
	for(i = 0; i < lens; i++){
        stampa_list_dettagliata(L[i]);
    }
}

/**
    Stampa un albero FitchTree 
    in modo ricorsivo
    @param t Albero di Fitch ( FitchTree )
    @param lens lunghezza dell'array di List dell'albero
*/
void print_FitchTree(FitchTree* t, int lens){

    if( t == NULL ){
        printf("Empty \n");
    }

    if( t->lista != NULL ){
        stampa_array_list(t->lista, lens);
        printf("\n");
    }
    else{
        printf("flag\n\n");
    }

    if( t->figli != NULL){
        print_FitchTree(t->figli, lens);
    }
    if( t->next != NULL){
        print_FitchTree(t->next, lens);
    }
    
}

/**
    Stampa un albero FitchTree 
    in modo ricorsivo in modo
    dettagliato (anche con i cont)
    @param t Albero di Fitch ( FitchTree )
    @param lens lunghezza dell'array di List dell'albero
*/
void print_FitchTree_dettagliata(FitchTree* t, int lens){

    if( t == NULL ){
        printf("Empty \n");
    }

    if( t->lista != NULL ){
        stampa_array_list_dettagliata(t->lista, lens);
        printf("\n");
    }
    else{
        printf("flag\n\n");
    }

    if( t->figli != NULL){
        print_FitchTree_dettagliata(t->figli, lens);
    }
    if( t->next != NULL){
        print_FitchTree_dettagliata(t->next, lens);
    }
    
}

/**
    Ritorna una lista di lunghezza i e con valori
    decrescenti a partire da i, il tutto in modo
    ricorsivo
    @param i lunghezza della lista ritornata
*/
List* make_list(int i) { 

    if( i <= 0 ){
        return NULL;
    }

    List *l; 
    while( i > 0 ){
	    l=(List*)malloc(sizeof(List));
        l->num=i;
        l->cont = 1;
	    l->next = make_list(i-1);
	    return l;
    }
    return l;
}

/**
    Ritorna un FitchTree da un Tree
    Cioe' trasforma un albero di stringhe in 
    un albero di array di liste (List).
    @param tree Albero di stringhe (Tree)
*/
FitchTree *make_FitchTree(Tree *tree){

    if( tree == NULL ){
        return NULL;
    }
    FitchTree *new_tree = (FitchTree *)malloc(sizeof(FitchTree));
    new_tree->padre = NULL;
    new_tree->n_figli = tree->nF;
    if( !tree->flag ){
        new_tree->lista = NULL;
    }
    else{
        new_tree->lista = (List**)malloc(strlen(tree->string)*sizeof(List));
        string_to_array_list(tree->string, new_tree->lista);
    }

    if( tree->figli != NULL ){
        Tree* temp_tree = tree->figli;
        new_tree->figli = make_FitchTree(temp_tree);
        FitchTree* temp = new_tree->figli;
        temp->padre = new_tree;
        new_tree->figli = temp;
        while( temp_tree->next != NULL ){
            temp->next = make_FitchTree(temp_tree->next);
            temp = temp->next; 
            temp->padre=new_tree;
            temp_tree = temp_tree->next;
            
        } 
    }
    else{
        new_tree->figli = NULL;
    }

    new_tree->next=NULL;

}

/**
    Ritorna una copia hard della lista
    @param b lista da copiare
*/
List* copy_list(List *b){

    if( b == NULL ){
        return NULL;
    }
    List *copy_p = (List*)malloc(sizeof(List));
    List *copy = copy_p;
    List *b_temp = b;
   
    do{
        copy->num = b_temp->num;
        copy->cont = b_temp->cont;
        if( b_temp->next != NULL ){
            copy->next = (List*)malloc(sizeof(List));
            copy = copy->next;
        }
        else{
            copy->next = NULL;
        }
        b_temp = b_temp->next;
    }while( b_temp != NULL);

    return copy_p;
}

/**
    Ritorna una copia hard dell'array di liste di
    lunghezza lens
    @param b array di liste da copiare
    @param lens lunghezza dell'array
*/
List ** copy_array_list(List *b[],int lens){

    List **copy = (List**)malloc(sizeof(List)*lens);
    int i;

    for(i = 0; i < lens; i++){
        copy[i] = copy_list(b[i]);
    }
    return copy;
}

/**
    Unisce a livello insiemistico a con b e salva in a
    il risultato assumendo che a e b siano di pari lunghezza lens.
    In caso a sia NULL crea una copia di b e la mette in a.
    @param a array di List in cui salva l'unione
    @param b array di List secondario
    @param lens lunghezza degli array di List
*/
List ** intersection(List *a[], List* b[],int lens){
    if( lens <= 0 ){
        return NULL;
    }

    if( a == NULL ){
        a = copy_array_list(b, lens);
    }
    else{
        for(int i = 0; i < lens; i++){
            if( list_len(a[i]) == 1 && list_len(b[i]) == 1 
                && a[i]->num == b[i]->num ){
                //vuol dire che in quella "cella" hanno un unico valore ed e' lo stesso
                a[i]->cont = a[i]->cont + b[i]->cont;
            }
            else{
                List *b_temp = b[i];
                while( b_temp != NULL ){
                    add_list_cont(a[i],b_temp->num,b_temp->cont);
                    b_temp = b_temp->next;
                }

            }
        }
    }
    return a;
}

/**
    Ritorna l'elemento piu' ripeturo nella lista,
    in caso di parita' ritorna il primo incontrato
    @param l lista 
    @param max_stati numero di stati massimo
*/
List* moda(List *l, int max_stati){

    if( l == NULL  ){
        return NULL;
    }
    int array[max_stati+1];
    int i;
    for(i = 0; i < max_stati+1; i++){
        array[i] = 0;
    }

    if( l->next == NULL ){

        List *new = (List*)malloc(sizeof(List)); 
        new->next = NULL; 
        new->num = l->num;
        new->cont = l->cont;
        return new;
    }
    
    while( l != NULL ){
        array[l->num] = array[l->num] + l->cont;
        l = l->next;
    }

    
    int index = 0;
    int max = array[0];
    for(i = 1; i < max_stati+1; i++){
        if( array[i] > max ){
            max = array[i];
            index = i;
        }
    }

    List *new = (List*)malloc(sizeof(List)); 
    new->next = NULL; 
    new->num = index;
    new->cont = max;
    return new;
}

/**
    Ritorna l'elemento piu' ripetuto nella lista
    tenendo in considerazione il valore del padre
    in caso di parita', se il padre non aiuta restituisce
    il primo incontrato.
    @param l lista
    @param max_stati numero di stati massimo
    @param t Nodo padre
    @param index_list Indice della lista che stiamo valutando

*/
List* moda_father(List *l, int max_stati, FitchTree *t, int index_list){

    if( l == NULL  ){
        return NULL;
    }
    int array[max_stati+1];
    int i;
    for(i = 0; i < max_stati+1; i++){
        array[i] = 0;
    }

    if( l->next == NULL ){

        List *new = (List*)malloc(sizeof(List)); 
        new->next = NULL; 
        new->num = l->num;
        new->cont = l->cont;
        return new;
    }
    
    while( l != NULL ){
        array[l->num] = array[l->num] + l->cont;
        l = l->next;
    }
    
    int index = 0;
    int max = array[0];
    for(i = 1; i < max_stati+1; i++){
        if( array[i] > max ){
            max = array[i];
            index = i;
        }
    }

    if( t != NULL && t->lista != NULL && 
        array[ t->lista[index_list]->num ] == max ){
        index = t->lista[index_list]->num;
    }

    List *new = (List*)malloc(sizeof(List)); 
    new->next = NULL; 
    new->num = index;
    new->cont = max;
    return new;
}

/**
    Applica un merge sull'array di liste a e lo salva in c.
    Il merge e' fatto eseguendo la funzione 'moda'
    @param a lista su cui fare merge
    @param c lista in cui salvare il risultato
    @param max_stati array che dice lo stato massimo per ogni List
    @param lens lunghezza degli array di List
*/
void merge(List *a[], List *c[], int max_stati[], int lens){

    int i;
    for(i = 0; i < lens; i++){
        c[i] = moda(a[i], max_stati[i]);
    }
}

/**
    True se tutti i gli sono con flag True,
    False altrimenti
    @param t Albero di Fitch ( FitchTree )
*/
bool all_children_flag(Tree* t){

    if( t == NULL || t->figli == NULL){
        return false;
    }

    Tree* f = t->figli;
    bool ris = f->flag;
    while( f->next != NULL && ris ){
        f = f->next;
        ris = ris && f->flag;
    }
    return ris;
}

/**
    Se tutti i figli hanno un valore not_NULL
    a lista return true, altrimenti false.
    In caso non abbia figli o esso stesso sia NULL
    ritorna false.
    @param t Albero di Fitch (FitchTree)
*/
bool all_children_not_NULL(FitchTree * t){
    if( t == NULL || t->figli == NULL){
        return false;
    }

    FitchTree* f = t->figli;
    bool ris = ( f->lista != NULL );
    while( f->next != NULL && ris  ){
        f = f->next;
        ris = ris && ( f->lista != NULL );
    }
    return ris;
}

/**
    Unisce a livelo insiemistico tutti i figli di t
    senza operare controlli
    @param t Albero di Fitch (FitchTree)
    @param num_caratteri Lunghezza degli array di List nell'albero
*/
FitchTree* join_f(FitchTree* t, int num_caratteri){
    FitchTree *f = t->figli;
    //t->lista == NULL; in questo punto.
    while( f != NULL ){
        t->lista = intersection(t->lista, f->lista, num_caratteri);
        f = f->next;
    }
    return t;
}

/**
    Esegue un'unione insiemistica sull'intero albero 
    Ritorna poi l'albero risultante
    @param t Albero di Fitch (FitchTreee)
    @param num_caratteri Lunghezza degli array di List nell'albero
*/
FitchTree* join_f_all(FitchTree* t, int num_caratteri){

    if( t == NULL ){
        return t; 
    }

    if( t->lista != NULL ){
        if( t->next != NULL ){
            t->next = join_f_all(t->next, num_caratteri);
        }
        else{
            return t;
        }
    }

    if( t->lista == NULL && all_children_not_NULL(t) ){
        t = join_f(t, num_caratteri);
    }
    else if(t->lista == NULL && all_children_not_NULL(t) == false) { 
        //Quindi t->lista == NULL && all_children_not_NULL(t) == false;
        t->figli = join_f_all(t->figli,num_caratteri);
        t = join_f_all(t, num_caratteri);
    }
    
    if( t->next != NULL ){
        t->next = join_f_all(t->next, num_caratteri);
    }
    return t;
}

/**
    Esegue una merge solo sulla radice dell'albero.
    @param t Albero di Fitch (FitchTree)
    @param num_caratteri Lunghezza degli array di List nell'albero
    @param max_stati array che dice lo stato massimo per ogni List

*/
void merge_root(FitchTree* t, int num_caratteri, int max_stati[]){
    if( t == NULL ){
        return;
    }
    int i;
    for(i = 0; i < num_caratteri; i++){
        t->lista[i] = moda(t->lista[i], max_stati[i]);
    }

}

/**
    Fa una merge completa per arrivare ad avere un albero con array 
    di liste con lunghezza uno (le liste).
    @param t Albero di Fitch (FitchTree)
    @param num_caratteri Lunghezza degli array di List nell'albero
    @param max_stati array che dice lo stato massimo per ogni List
*/
void merge_all(FitchTree* t, int num_caratteri, int max_stati[]){
    if( t == NULL ){
        return;
    }

    if( t->lista != NULL ){
        int i;
        for(i = 0; i < num_caratteri; i++){
            t->lista[i] = 
                moda_father(t->lista[i], max_stati[i]+1, t->padre, i);
        }      
    }

    if( t->figli != NULL ){
        merge_all(t->figli, num_caratteri, max_stati);
    }

    if( t->next != NULL ){
        merge_all(t->next, num_caratteri, max_stati);
    }

}

/**
    Riceve un FitchTree con lista di profondità massima 1,
    cioe' risolta, e ritorna un albero di Newick equivalente
    @param new_tree Albero di Newick (Tree)
    @param tree Albero di Fitch risolto (FitchTree)
    @param num_caratteri Lunghezza degli array di List nell'albero
*/
Tree * FitchTree_to_Newick(Tree *new_tree, FitchTree *tree, int num_caratteri){

    if( tree == NULL ){
        return NULL;
    }
    new_tree->padre = NULL;
    new_tree->nF = tree->n_figli;
    new_tree->flag = true;
    new_tree->string = array_list_to_string(tree->lista, num_caratteri);
    if( tree->figli != NULL ){
        new_tree->figli = FitchTree_to_Newick(new_tree->figli, tree->figli, num_caratteri);
        new_tree->figli->padre = new_tree; 
    }
    else{
        new_tree->figli = NULL;
    }
    if( tree->next != NULL ){
        new_tree->next = FitchTree_to_Newick(new_tree->next, tree->next, num_caratteri); 
        
    }
    else{
        new_tree->next = NULL;
    }

    return new_tree;
}


/**
    Risolve l'albero eseguendo l'algoritmo di 
    Fitch
    @param t Albero di Newick (Tree)
    @param num_caratteri Lunghezza delle stringhe nell'albero
    @param max_stati Array che dice lo stato massimo per ogni carattere
*/
void Fitch(Tree* t, int num_caratteri, int max_stati[]){
    
    if( t == NULL ){
        return; 
    }   
    FitchTree* fitch = make_FitchTree(t);
    fitch = join_f_all(fitch, num_caratteri);
    merge_all(fitch, num_caratteri, max_stati);
    FitchTree_to_Newick(t, fitch, num_caratteri);
    
}

/**
    Salva la stringa in un file con nome simile a quello in input.
    Se ritorna false c'e' stato un errore.
    @param path Percorso del file
    @param string Stringa che si vuoe salvare nel file
*/
bool string_to_file(char *path, char* string){
    int len_path = len(path);
    char *newpath = (char*)malloc((len_path+5)*(sizeof(char)));
    strcpy(newpath,path);
    
    //Togliamo .txt finale
    newpath[len_path-1] = '\0';
    newpath[len_path-2] = '\0';
    newpath[len_path-3] = '\0';
    newpath[len_path-4] = '\0';
    strcat(newpath,"Fitch.txt");

    FILE *file;
    file=fopen(newpath, "w");
    if( file==NULL ) {
        perror("Errore in apertura del file");
        return false;
    }

	/* scrive il numero */
    fprintf(file, "%s\n", string);
    /* chiude il file */
    fclose(file);
    return true;
}

/**
    Algoritmo di Fitch
    Il path deve esser del tipo:
    -   /home/davide/Scrivania/nome_file.txt
    -   nome_file.txt
    @param path Percorso del file 
*/
void fitch_algorithm(char* path){

    char *stringa = fstring(path);
    printf("Input Tree: %s\n",stringa);
    Tree *newick = Newick(stringa);
    int numero_caratteri = numeroCaratteri(stringa);
    int *max_stati = statoMaxPerCarattere(stringa);
    Fitch(newick, numero_caratteri, max_stati);
    char* stringa_newick = treeToNewick(newick);
    printf("Output Fitch Tree: %s \n",  stringa_newick);    
    string_to_file(path,stringa_newick);
}
