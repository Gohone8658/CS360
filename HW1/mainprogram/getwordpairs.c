#include "functions.h"

// void processWordPairs(FILE file[]){

//     int endOfFileFlag = 0;
//     char *temp = getNextWord(file);
//     char *firstWord = temp;
//     while(endOfFileFlag != 1){
//         //char *firstWord = getNextWord(file);
//         char *secondWord = getNextWord(file);
//         char pair[257] = "";

//         if (firstWord != NULL && secondWord != NULL){
//             makeWordPair(firstWord, secondWord, pair);
//             //hashFind()
//             //printf("<%s> ", pair);
//         }
//         if(firstWord == NULL || secondWord == NULL){
//             endOfFileFlag = 1;
//         }
//         firstWord = secondWord;
//     }
// }

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