
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include <string.h> //Per manipolare le stringhe. 
#include <stdlib.h>
#include "Parser.h"

typedef struct List {
	int num;
    int cont;
	struct List *next;
}List;


List* doList(int i){ 
    /* Ritorna una lista con una cella sola e valore i */

    List *l = (List*)malloc(sizeof(List));
    l->num = i;
    l->cont = 1;
    l->next = NULL;

    return l;
}

int ListLen(List *l) {
    /* Ritorna la lunghezza della lista */

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

List* last(List* l){ 
    /* Ritorna l'ultimo elemento della lista */

    if (l == NULL) {
        return NULL;
    }
    List *new = l;
    while (new->next != NULL) {
        new = new->next;
    }
    return new;
}

int myAtoi(char c){
    /* converte il char numerico in un numero '9' -> 9
    funziona solo con numeri in [0,9] */
    int num = c - '0';
    return num;
}

List* stringToList(char *string){
    /*Converte una stringa in una List */

    int lenS = len(string);
    if( lenS < 1 ){
        return NULL;
    }
    List *temp; 
    List *new = doList(myAtoi(string[0]));
    List *act = new;
    int i;
    for(i = 1; i < lenS; i++){
        temp = doList(myAtoi(string[i]));
        act->next = temp;
        act = act-> next;
    }
    return new;
}

void stringToArrayList(char* string, List* arrayList[]){
    /*Converta una stringa in un array di liste di lunghezza uno */

    int lenS = len(string);
    if( lenS < 1 ){
        arrayList = NULL;
    }
    int i;
    
    for(i = 0; i < lenS; i++){
        arrayList[i] = doList(myAtoi(string[i]));
    }
}

char* arrayListToString(List* arrayList[], int lenS){
    /* Converta un array di liste di lunghezza uno in una stringa
    ATTENZIONE la lunghezza di arrayList deve esser lenS*/

    
    int i;
    char *string = (char*)malloc(sizeof(char)*(lenS+1));
    string[lenS] = '\0';
    for(i = 0; i < lenS; i++){
        string[i] = arrayList[i]->num + '0';
    }
    return string;
}

void ArrayListcopy(List* b[], List* a[], int lens){
    /*Copia a in b */
    int i;
    for(i = 0; i < lens; i++){
        b[i] = doList(a[i]->num);
    }
}

void stampaList(List *l){
    /*Stampa una List */

    if (l == NULL) {
        printf("empty \n");
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

void stampaListDettagliato(List *l){
    /*Stampa una List con anche il cont */

    if (l == NULL) {
        printf("empty \n");
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

void stampaArrayList(List* L[], int lens) {
    /*Stampa un'array di List di lunghezza lens 
    (oppure fino a lens) */

    int i;
	for(i = 0; i < lens; i++){
        stampaList(L[i]);
    }
}

void stampaArrayListDettagliato(List* L[], int lens) {
    /*Stampa un'array di List di lunghezza lens con il relativo cont
    (oppure fino a lens) */

    int i;
	for(i = 0; i < lens; i++){
        stampaListDettagliato(L[i]);
    }
}

List* makeList(int i) { 
    /* Crea una lista di lunghezza i tramite ricorsione */
    if( i <= 0 ){
        return NULL;
    }

    List *l; 
    while( i > 0 ){
	    l=(List*)malloc(sizeof(List));
        l->num=i;
        l->cont = 1;
	    l->next = makeList(i-1);
	    return l;
    }
    return l;
}

void intersection(List *a[], List* b[],int lens){
    /* Interseca a livello insiemistico a con b e salva in a
    Assume che siano di pari lunghezza lens*/

    if( lens <= 0 ){
        return ;
    }
    
    for(int i = 0; i < lens; i++){
        if( ListLen(a[i]) == 1 && ListLen(b[i]) == 1 && a[i]->num == b[i]->num ){
            //vuol dire che in quella "cella" hanno un unico valore ed e' lo stesso
            a[i]->cont = a[i]->cont + b[i]->cont;
        }
        else{
            last(a[i])->next = b[i];
        }
    }
    
}

List* moda(List *l, int max_stati){
    /* Ritorna l'elemento piu' ripetuto nella lista, 
    in caso di parita' ritorna il primo incontrato */

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

void merge(List *a[], List *c[], int max_stati[], int lens){
    /* Applica moda su ogni lista dell'array a e la metta in c 
    l'output (c) sara' dunque un array di liste di dimensione 1 */

    int i;
    for(i = 0; i < lens; i++){
        c[i] = moda(a[i], max_stati[i]);
    }
}

bool allChildrenFlag(Tree* t){
    /* True se tutti i figli sono con flag True
    False altrimenti*/

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

void Fitch(Tree* t, int num_caratteri, int maxStati[]){
    /*Risolve l'albero t eseguendo l'algoritmo di Fitch */
    
    if( t == NULL ){
        return; 
    }
    
    if( t->flag ){
        //dunque se t e' risolto completamente
        return;
    }

    if( allChildrenFlag(t) ){ 
        //(info) se non ha figli non entra nemmeno

        int numFigli = t->nF;
        Tree* f = t->figli;
        if( numFigli == 1 ){
            if( f->flag ){
                strcpy(t->string, f->string); 
                /*Si potrebbe fare t->string = f->string
                e si ottimizzerebbe la memoria ma 
                ciò porta ad un legame troppo stretto 
                tra padre e figlio, per modellazioni 
                future considero meglio fare cosi*/
                t->flag = true;
                return;
            }
            else{
                Fitch(f, num_caratteri, maxStati);
                Fitch(t, num_caratteri, maxStati);
                return;
            }
            
        }
        int i;
        List *a[num_caratteri];
        List *b[num_caratteri];
        List *c[num_caratteri];

        stringToArrayList(f->string, a);
        while( f->next != NULL ){
            f = f->next;
            stringToArrayList(f->string, b);
            intersection(a,b,num_caratteri);
        }

        merge(a,c, maxStati, num_caratteri);
        t->string = arrayListToString(c, num_caratteri);
        t->flag = true;
        Fitch(t, num_caratteri, maxStati);
        return;
    }

    if( t->figli != NULL ){
        Tree* f = t->figli;

        while( f != NULL ){
            if( !f->flag ){
                Fitch(f, num_caratteri, maxStati);
                f->flag = true;
            }
            f = f->next;
        }
    }

    Fitch(t, num_caratteri, maxStati);    
}


bool stringToFile(char *path, char* string){
    /*Salva la stringa in un file simile a quello inserito */
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

void fitch_algorithm(char* path){
    /*Algoritmo di Fitch
    Il path deve esser del tipo:
    -   /home/davide/Scrivania/nome_file.txt
    -   nome_file.txt
    */

    char *stringa = fstring(path);
    printf("Input Tree: %s\n",stringa);
    Tree* newick = Newick(stringa);
    int numero_caratteri = numeroCaratteri(stringa);
    int *max_stati = statoMaxPerCarattere(stringa);
    Fitch(newick, numero_caratteri, max_stati);
    char* stringaNewick = treeToNewick(newick);
    printf("Output Tree: %s \n",  stringaNewick);    
    stringToFile(path,stringaNewick);
}
