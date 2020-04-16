#include <math.h> //Per le funzioni matematiche comuni. 
#include <stdlib.h>
#include <stdarg.h> //Utilizzato da funzioni che accettano un numero variabile di parametri. 
#include <stdbool.h> //Per un tipo di dato booleano. (Aggiunto con il C99) 
#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include "Parser.h"

#define size(arr) (sizeof(arr) / sizeof(arr[0]))



typedef struct SankoffTree {
    int nF; //numero figli
	int* score;
    struct SankoffTree* padre;
    struct SankoffTree* figli; //punta al primo figlio
    struct SankoffTree* next; //punta al prossimo fratello
}SankoffTree;

const int inf = __INT_MAX__/16;












void printScore(int n, int *score[n])
{
    printf("(");
    if (score != NULL)
        for (int i = 0; i < n; i++)
        {
            printf("%d,", score[0][i]);
        }
    printf(")\n");
}

void printMe(SankoffTree *stree, int i, int n)
{
    printf("%d: ", i);
    i++;
    printScore(n, stree->score);
    int nf = stree->nF;
    if (nf > 0)
    {
        stree = stree->figli;
        printf("%d: ", i);
        printScore(n, stree->score);
        for (int i2 = 1; i2 < nf; i2++)
            printMe(stree->next, i + i2, n);
    }
}

















// crea un albero con la stessa morfolofia di quello in input e che ne inizializza i costi delle foglie per
// il carattere di indice character
SankoffTree* makeSankoffTree(Tree* tree, int character, int totStates){
    SankoffTree *stree = (SankoffTree*)malloc(sizeof(SankoffTree));
    stree->nF = tree->nF;
    
    //se il nodo originale non ha informazioni circa il carattere allora neanche il nodo del nuovo albero ne avrà, altrimenti
    //si imposterà a 0 il costo per lo stato che assume il carattere in quel nodo e +infinito per tutti gli altri stati 
    if(tree->string[0] < 48 || tree->string[0] > 57){
        stree->score = NULL;
    }else{
        int* score = malloc(sizeof(int)*totStates); // inizializza array score di int, lunghezza totStates
        //printf("stringa: %s, ",tree->string);
        //printf("carattere: %c\n",tree->string[character] );
        for(int i = 0; i< totStates; i++){
            if(i == tree->string[character] - '0')
                score[i] = 0;
                else
                score[i] = inf;  
        }
        stree->score = score;
        //printScore(totStates, score);
    }
   stree->padre = NULL;
   
   //se il nodo ha almeno un figlio si costruiscono anche i nodi dei figli
   if(stree->nF > 0){
    //son sarà il primo figlio
    SankoffTree* son = makeSankoffTree((tree->figli), character, totStates);
    son->padre = stree;
    stree->figli = son;

    //fino a quando non si creano tutti i figli si crea un nuovo nodo e viene inizializzato sulla base di quello equivalente nell'albero originale
    //si specifica che il padre di questi figli sarà stree
    Tree* sontree = tree->figli;
    for(int i = 1; i < tree->nF; i++){
            SankoffTree* son2 = makeSankoffTree(sontree->next, character, totStates);
            son2->padre = stree;
            son->next = son2;
            sontree = sontree->next;
            son = son2;
   }
   }else{
       stree->figli = NULL;
   }
   stree->next = NULL;
}


//effettua il calcolo del parsimony score dati i figli
int *join(SankoffTree* nodes, int totStates, int cost[totStates][totStates]){
   int *joined = malloc(totStates*sizeof(int));
    int numBrothers = nodes->padre->nF;

   for(int i = 0; i < totStates; i++){

      joined[i] = 0;
      int mins[numBrothers];

      for(int j = 0; j < numBrothers; j++) mins[j] = inf;

      //  printScore(totStates,nodes->score);

      for(int j = 0; j < totStates; j++){

//printf("costo da %d a %d =  %d\n",i,j,cost[i][j]);
          if(mins[0] > cost[i][j] + nodes->score[j])
             mins[0] = cost[i][j] + nodes->score[j];

             //printf("mins[%d]: %d\n",0,mins[0]);
             
             SankoffTree* next = nodes->next;
         for( int n = 1; n < numBrothers; n++){

             //printScore(totStates,next->score);

            if(mins[n] > cost[i][j] + next->score[j])
               mins[n] = cost[i][j] + next->score[j];
                //printf("mins[%d]: %d\n",n,mins[n]);
             next = next->next;
         }
      }

//printf("\nminimi per stato %d: (",i);
     //    for(int loop = 0; loop < numBrothers; loop++)
      //  printf("%d,", mins[loop]);

       // printf(")\n");    
     for(int j = 0; j < numBrothers; j++)
        joined[i] = joined[i] + mins[j];
       // printf("joined[%d] = %d\n",i,joined[i]);
   }
//printf("joined: (");
    //for(int loop = 0; loop < totStates; loop++)
        //printf("%d,", joined[loop]);
//printf(")");
   return joined;
}



//per ogni nodo se non ha lo score lo calcola sulla base di quello dei figli (procedura ricorsiva)
void *Sankoff(SankoffTree* node, int totStates, int cost[totStates][totStates]){

    //printf("\n %d -- \n",node->nF);
    //for(int loop = 0; loop < totStates; loop++)
    //if(node->score != NULL)
        //printf("\n%d ", node->score[loop]);
    if(node->score == NULL){
       SankoffTree* son = node->figli;
        Sankoff(son, totStates, cost);
        for(int i = 1; i < node->nF; i++){
            SankoffTree* son2 = son->next;
            Sankoff(son2, totStates, cost);
            son = son2;
        }
    node->score = join(node->figli, totStates, cost);
    }
}


Tree *STreeToTree(SankoffTree *stree, int totStates){
     Tree *tree = (Tree*)malloc(sizeof(Tree));
    tree->nF = stree->nF;
    if(stree->score == NULL){
        tree->string = NULL;
    }else{   
        int min = 0;
        for(int i = 1; i< totStates; i++)
            if(stree->score[i]<stree->score[min])
                min = i;    

         char *stringa = malloc(2*sizeof(char));
         stringa[1] =  '\0';
         stringa[0] = min + '0';
        tree->string = stringa;
    }
   tree->padre = NULL; 
   //se il nodo ha almeno un figlio si costruiscono anche i nodi dei figli
   if(tree->nF > 0){
    
    //son sarà il primo figlio
    Tree* son = STreeToTree((stree->figli), totStates);
    son->padre = tree;
    tree->figli = son;

    //fino a quando non si creano tutti i figli si crea un nuovo nodo e viene inizializzato sulla base di quello equivalente nell'albero originale
    //si specifica che il padre di questi figli sarà stree
    SankoffTree* sontree = stree->figli;
    for(int i = 1; i < stree->nF; i++){
            Tree* son2 = STreeToTree(sontree->next, totStates);
            son2->padre = tree;
            son->next = son2;
            sontree = sontree->next;
            son = son2;
   }
   }else{
       tree->figli = NULL;
   }
   tree->next = NULL;
    return tree;
}


void printScore(int n, int score[n]){
    printf("(");
    if(score!=NULL)
for(int i = 0; i<n; i++){
printf("%d,",score[i]);
}
printf(")\n");
}
void printMe(SankoffTree *stree,int i, int n){
printf("%d: ",i);
i++;
printScore(n, stree->score);
int nf = stree->nF;
if (nf > 0){
stree = stree->figli;
printf("%d: ",i);
printScore(n, stree->score);
for (int i2 = 1; i2 < nf; i2++)
printMe(stree->next, i+i2, n);
}
}

void sankoff_algorithm(char *stringa){
    
    printf("%s \n",stringa);
    Tree* newick = Newick(stringa);

   //printTree(newick);

    int n = 5;
   int cost[5][5] = {{0,1,1,1,1},{1,0,1,1,1},{1,1,0,1,1},{1,1,1,0,1},{1,1,1,1,0}};

   //for(int i = 0;i<n;i++){
  // printf("\n");
  // for(int j=0;j<n;j++)
  // printf("%d,",cost[i][j]);
   //}

   SankoffTree* stree = makeSankoffTree(newick, 0, n);

  // printf("\n Inizio! \n");
//printMe(stree, 0, n);
     //  printf("\n Fine! \n");

    Sankoff(stree,n,cost);
    
     //  printf("\n Inizio! \n");
 // printMe(stree, 0, n);
  // printf("\n Fine! \n");


    newick = STreeToTree(stree, n);

    char* stringaNewick = treeToNewick(newick);

    printf("%s \n",  stringaNewick);
}


int main(){
    char *stringa = fstring("newick.txt");
    sankoff_algorithm(stringa);
}