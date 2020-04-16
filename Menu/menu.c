#include <stdio.h> //Fornisce le funzionalità basilari di input/output del C. Questo file include il prototipo delle venerabili funzioni printf e scanf. 
#include <string.h> //Per manipolare le stringhe. 
#include <stdbool.h> 
#include <stdlib.h>
#include "Fitch.h"
#include "Sankoff.h"


void caseFitch(){
    /*Si occupa di prendere in input il path*/
    
    printf("Fitch Algorithm\n");
    char path[10000];
    printf("Insert the path in witch there is the Newick Tree\n");
    printf("The path should not be greater than 10000\n");
    scanf("%s",&*path);
    fitch_algorithm(path);
}

void caseSankoff(){
    /*Si occupa di prendere in input il path*/
    
    printf("Sankoff Algorithm\n");
    char path[10000];
    char pathM[10000];
    printf("Insert the path in witch there is the Newick Tree\n");
    printf("The path should not be greater than 10000\n");
    scanf("%s",&*path);
    printf("Insert the path in witch there is the Matrix\n");
    printf("The path should not be greater than 10000\n");
    scanf("%s",&*pathM);
    sankoff_algorithm(path, pathM);
}

void caseBoth(){
    /*Si occupa di prendere in input il path*/

    printf("Sankoff and Fitch Algorithms\n");
    char path[10000];
    char pathM[10000];
    printf("Insert the path in witch there is the Newick Tree\n");
    printf("The path should not be greater than 10000\n");
    scanf("%s",&*path);
    printf("Insert the path in witch there is the Matrix\n");
    printf("The path should not be greater than 10000\n");
    scanf("%s",&*pathM);
    sankoff_algorithm(path, pathM);
    fitch_algorithm(path);
}

void clrscr(){
    /*Pulisce il terminal */
    system("@cls||clear");
}

void menu(){
    printf("This is a C implementation of Sankoff and Fitch Algorithms\n");
    printf("by Davide Pietrasanta and Pietro Tropeano \n");
    char input;
    
    printf("Press: \n");
    printf("0 for both Sankoff and Fitch Algorithms \n");
    printf("1 for Sankoff Algorithm \n");
    printf("2 for Fitch Algorithms \n");
    printf("Anything else to exit\n");

    scanf("%c", &input);
    //clrscr();
    switch(input){
        case '0' : 
            caseBoth();
            break;
        case '1' : 
            caseSankoff(); 
            break;
        case '2' : 
            caseFitch();
            break;
        default: 
            printf("End\n");
    }


}

int main(){
    menu();
}