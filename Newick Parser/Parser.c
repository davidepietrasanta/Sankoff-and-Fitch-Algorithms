
#include <math.h> //Per le funzioni matematiche comuni. 
#include <stdlib.h>
#include <stdarg.h> //Utilizzato da funzioni che accettano un numero variabile di parametri. 
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include <string.h> //Per manipolare le stringhe. 

typedef struct Tree {
    int nF; //numero figli
	char* string;
    struct Tree* padre;
    struct Tree* figli; //punta al primo figlio
    struct Tree* next; //punta al prossimo fratello
}Tree;



char* fstring(char *path){
    /* Legge un file e ritorna una stringa del file */

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

Tree* doTree(char* c){
    /* Ritorna un Tree con valore c */

    Tree *l = (Tree*)malloc(sizeof(Tree));
    l->nF = 0;
    l->string = c;
    l->padre = NULL;
    l->figli = NULL;
    l->next = NULL;

    return l;
}

void addNext(Tree* t, Tree* brother){
    /* Aggiunge brother come next di t, 
    in caso ci sia gia' un next va in fondo */

    if( brother == NULL ){
        return;
    }
    if( t == NULL ){
        t = brother;
        return;
    }
    brother->padre = t->padre;
    if( t->padre != NULL ){
        brother->padre->nF = brother->padre->nF + 1;
    }
    while(t->next != NULL){
        t = t->next;
    }
    t->next = brother;
}

int len(char *s){ 
    /* Restituisce la lunghezza della stringa s */

    int i;
    for(i = 0; *(s+i) != '\0' ; i++)
        ;
    return i;
} 

char* reverse(char *s){
    /* Restituisce la stringa inversa della stringa s */

    int lenS = len(s);
    int i;
    char *temp = (char*)malloc((lenS+1)*sizeof(char));
    for(i = 0; i < lenS; i++){
        if( s[lenS-1-i] == ')' || s[lenS-1-i] == '(' ){
            if( s[lenS-1-i] == ')' ){
                temp[i] = '(';
            }
            else{
                temp[i] = ')';
            }
        }
        else{
            temp[i] = s[lenS-1-i];
        }
    }
    temp[lenS] = '\0';
    return temp;
}

int contSimbol(char *s){
    /* Conta i simboli in s
    simboli = '(' , ')' oppure ',' */

    int i;
    int cont=0;
    int lenS = len(s);
     for(i = 0; i < lenS; i++){
        if( s[i] == ')' || s[i] == '(' || s[i] == ',' ){
            cont++;
        }
    }
    return cont;
}

int* allSimbol(char *s){
    /* Ritorna un array contenente la lista
    degli indici, in s, dove ci sono simboli
    simboli = '(' , ')' oppure ','  */

    int cont = contSimbol(s);
    int contTemp = 0;
    int i;
    int *array = (int*)malloc((cont)*sizeof(int));
    for(i = 0; s[i] != '\0' ; i++){
        if( s[i] == ')' || s[i] == '(' || s[i] == ',' ){
            array[contTemp] = i;
            contTemp++;
        }
    }
    return array;
}

void setInit(char *s, int i){
    /* Setta la stringa s di lunghezza i 
    (oppure fino ad i) */

    int j;
    for(j = 0; j < i; j++){
        s[j] = '\0';
    }
}

char** extract(char *rev){
    /* Ritorna un array di stringhe contenete tutto cio'
    compreso tra due simboli */

    int i;
    int cont = contSimbol(rev);
    int *array = allSimbol(rev); //lunghezza cont
    char **estratti = (char**)malloc( cont * sizeof(char*));
    for(i =0; i < cont; i++){
        estratti[i] = (char*)malloc( len(rev) * sizeof(char)); //just to be sure...
        setInit(estratti[i], len(rev));
    }
    
    strncpy(estratti[0], rev, array[0]);
    for(i = 1; i < cont-1; i++){
        strncpy(estratti[i], rev+array[i-1]+1, array[i]-array[i-1]-1);
    }
    strncpy(estratti[i], rev+array[cont-2]+1, 1);

    return estratti;
}

Tree* Newick( char* string){
    /* Ritorna l'albero relativo alla stringa 
    secondo la codifica Newick*/

    Tree* root = (Tree*)malloc(sizeof(Tree));
    char *rev = reverse(string);
    int cont = contSimbol(rev);
    char **estratto = extract(rev);
    int *indexSimbols = allSimbol(rev); //lunghezza cont
    int i; 
    root = doTree(estratto[0]);

    for(i = 0; i < cont-1; i++){
        if( rev[indexSimbols[i]] == '(' ){
            if( root->figli == NULL ){
                root->figli = doTree(estratto[i+1]);
                root->nF = root->nF + 1;
                root->figli->padre = root;
                root = root->figli;
            }
            else{
                root = root->figli;
                addNext(root,doTree(estratto[i+1]));
            }
        }
        if( rev[indexSimbols[i]] == ',' ){
            addNext(root, doTree(estratto[i+1]));
            if( root->next != NULL){
                root = root->next;
            }
        }
        if( rev[indexSimbols[i]] == ')' ){
            root = root->padre;
        }
    }
    return root->padre;
}

void printIntArray(int *array, int len){
    /* Stampa un array di int di lunghezza len
    (oppure fino a len) */

    int i;
    for(i = 0; i < len-1; i++){
        printf("%d, ",array[i]);
    }
    printf("%d \n ",array[len-1]);
}

void stampaMatrix(char **s, int x){
    int i;
    for(i = 0; i < x-1; i++){
        printf("%s, ",s[i]);
    }
    printf("%s\n", s[x-1]);
}

void printTree(Tree* t){
    if( t == NULL ){
        printf("Empty \n");
    }

    printf("%s \n", t->string);
    if( t->figli != NULL){
        printTree(t->figli);
    }
    if( t->next != NULL){
        printTree(t->next);
    }
}


int main(){ 
    
    char *stringa = fstring("newick.txt");
    char *stringa_rev = reverse(stringa);
    printf("%s \n",stringa);
    printf("%s reverse\n",stringa_rev);
    printIntArray(allSimbol(stringa_rev), contSimbol(stringa_rev));
    char **matrix = extract(stringa_rev);
    stampaMatrix(matrix, contSimbol(stringa_rev));
    printTree(Newick(stringa));
    
    return 0;
}