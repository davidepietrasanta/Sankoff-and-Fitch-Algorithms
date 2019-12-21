
#include <math.h> //Per le funzioni matematiche comuni. 
#include <stdarg.h> //Utilizzato da funzioni che accettano un numero variabile di parametri. 
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include <string.h> //Per manipolare le stringhe. 

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

void intersection(List *a[], List* b[], List* c[],int len){
    /* Interseca a livello insiemistico a con b e salva in c */

    //Posso assumere siano di pari lunghezza len
    //Ho un Array di List
    if( len <= 0 ){
        return ;
    }

    for(int i = 0; i < len; i++){
        if( ListLen(a[i]) == 1 && ListLen(b[i]) == 1 && a[i]->num == b[i]->num ){
            //vuol dire che in quella "cella" hannno un unico valore ed e' lo stesso
            c[i]->num = a[i]->num;
            c[i]->cont = c[i]->cont + 2;
        }
        else{
            c[i] = a[i];
            last(c[i])->next = b[i];
        }
    }
}

List* moda(List *l, int max_stati){
    /* Ritorna l'elemento piu' ripetuto nella lista, 
    in caso di parita' ritorna il primo incontrato */

    if( l == NULL || max_stati <= 0 ){
        return NULL;
    }
    int array[max_stati];
    int i;
    for(i = 0; i < max_stati; i++){
        array[i] = 0;
    }
    while( l->next != NULL ){
        array[l->num] = array[l->num] + l->cont;
        l = l->next;
    }
    if( l != NULL ){
        array[l->num] = array[l->num] + l->cont;
    }

    int index = 0;
    int max = array[0];
    for(i = 1; i < max_stati; i++){
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

void merge(List *a[], List *c[], int max_stati[], int len){
    /* Applica moda su ogni lista dell'array a e la metta in c 
    l'output (c) sara' dunque un array di liste di dimensione 1 */

    int i;
    for(i = 0; i < len; i++){
        c[i] = moda(a[i], max_stati[i]);
    }
}




main(){ 
    printf("work in progress!! \n ");
}