
#include <math.h> //Per le funzioni matematiche comuni. 
#include <stdlib.h>
#include <stdarg.h> //Utilizzato da funzioni che accettano un numero variabile di parametri. 
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include <string.h> //Per manipolare le stringhe. 

typedef struct Tree {
    int nF; //numero figli
	char* string;
    bool flag; //True if ok
    struct Tree* padre;
    struct Tree* figli; //punta al primo figlio
    struct Tree* next; //punta al prossimo fratello
}Tree;


int len(char *s){ 
    /* Restituisce la lunghezza della stringa s */

    int i;
    for(i = 0; *(s+i) != '\0' ; i++)
        ;
    return i;
} 

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

bool numericalString(char *s){
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

Tree* doTree(char* c){
    /* Ritorna un Tree con valore c */

    Tree *l = (Tree*)malloc(sizeof(Tree));
    l->nF = 0;
    l->string = c;
    l->flag = numericalString(c);
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

void printIntArray(int *array, int len){
    /* Stampa un array di int di lunghezza len
    (oppure fino a len) */

    int i;
    for(i = 0; i < len-1; i++){
        printf("%d, ",array[i]);
    }
    printf("%d \n",array[len-1]);
}

char** extract(char *rev){
    /* Ritorna un array di stringhe contenete tutto cio'
    compreso tra due simboli */

    int i;
    int cont = contSimbol(rev);
    int *array = allSimbol(rev); //lunghezza cont
    //printf("simbol ");printIntArray(array, cont);
    char **estratti = (char**)malloc( cont * sizeof(char*));
    for(i =0; i < cont; i++){
        estratti[i] = (char*)malloc( len(rev) * sizeof(char)); //just to be sure...
        setInit(estratti[i], len(rev));
    }
    
    strncpy(estratti[0], rev, array[0]);
    //printf("Estratti[0] %s \n",estratti[0]);
    for(i = 1; i < cont-1; i++){
        strncpy(estratti[i], rev+array[i-1]+1, array[i]-array[i-1]-1);
    }
    strncpy(estratti[i], rev+array[cont-2]+1, array[cont-1]-array[cont-2]-1);
    //printf("Estratti[i] %s \n",estratti[i]);

    return estratti;
}

void stampaMatrix(char **s, int x){
    int i;
    for(i = 0; i < x-1; i++){
        printf("%s, ",s[i]);
    }
    printf("%s\n", s[x-1]);
}

Tree* Newick(char* string){
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

int numeroNodi(Tree *t){
    if( t == NULL ){
        return 0;
    }

    int cont = 1;
    if( t->figli != NULL){
        cont = cont + numeroNodi(t->figli);
    }
    if( t->next != NULL){
        cont = cont + numeroNodi(t->next);
    }
    return cont;
}

int numeroCaratteri(char *string){
    
    char ** estratti = extract(string);
    int nSimboli = contSimbol(string);
    int max = 0;
    if(string == NULL ){
        return 0;
    }
    int i;
    for(i = 0; i < nSimboli; i++){
        if( numericalString(estratti[i]) && max < len(estratti[i]) ){
            max = len(estratti[i]);
        }
    }
    return max;
}

int* statoMaxPerCarattere(char *string){

    if(string == NULL ){
        return 0;
    }

    char ** estratti = extract(string);
    int nSimboli = contSimbol(string);
    int nCaratteri = numeroCaratteri(string);
    int *max = (int*)malloc(nCaratteri*sizeof(int)); 
    //int max[nCaratteri];
    int i;
    int j;
    for(i = 0; i < nCaratteri; i++){
        max[i] = 0;
    }

    for(i = 0; i < nSimboli; i++){
        if( numericalString(estratti[i]) ){
            for(j = 0; j < nCaratteri; j++){
                if( max[j] < estratti[i][j] - '0' ){
                    max[j] = estratti[i][j] - '0';
                }
            }   
        }
    }
    

    //stampaMatrix(estratti, nSimboli);
    //printf("estratti[0] %s.\n", estratti[0]);

    //printf("max ");
    //printIntArray(max, nCaratteri);
    return max;
}

char* treeToNewick(Tree *t){

    if( t == NULL ){
        return NULL;
    }

    char *res = t->string;
    //printf("res: %s\n", res);
    if( t->figli != NULL){
        res = strcat(res, "(");
        res = strcat(res, reverse(treeToNewick(t->figli)));
        res = strcat(res, ")");
        
    }
    if( t->next != NULL){
        res = strcat(res, ",");
        res = strcat(res, reverse(treeToNewick(t->next)));
    }
    res = reverse(res);
    return res;
}

