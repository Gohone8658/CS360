#include "functions.h"

//prints each struct's 'occurrences' value from a given struct array.
void printWordPairs(struct node *Arr, int wordPairsToReturn, int numNodes){

    if(wordPairsToReturn > 0 && wordPairsToReturn < numNodes){
        for(int i = 0; i < wordPairsToReturn; i++){
            char *printPair = Arr[i].pair;
            int printOccur = Arr[i].occurrences;
            printf("%10d %s\n", printOccur, printPair);
        }
    }
    else{
            for(int i = 0; i < numNodes; i++){
            char *printPair = Arr[i].pair;
            int printOccur = Arr[i].occurrences;
            printf("%10d %s\n", printOccur, printPair);
        }
    }
}

//takes in two strings and concatenates them with a whitespace character in the middle.
char *makeWordPair(char *firstWord, char *secondWord, char *wordPair){
    char *whiteSpace =" ";
    char buffer[257]="";

    strcat(buffer, firstWord);
    strcat(buffer, whiteSpace);
    strcat(buffer, secondWord);
    strcpy(wordPair, buffer);
    buffer[0] = 0;
    return(wordPair);
}