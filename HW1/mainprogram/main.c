//Names: John Gregerson --- Class: CS360 --- Assignment: HW1 --- Date 09/07/2022

//program files context: (crc64.c, crc64.h, getWord.c, and getWord.h) are course provided and implement a hash algorithm
//and getword function respectively. (functions.h) includes the function definitions for both getwordpairs.c and hashtable.c
//(getwordpairs.c) contains functions for parsing ingested text into wordpairs. (hashtable.c) contains functions for
//the initialization and interaction with a linked list implemented hashtable.

//This program will allow the user to read in words from one or multiple files and
//output a list of a user specified number of most frequently appearing sequential pairs of words.

//Implemented via a hasharray assigning each location in the array to a sentinel node to begin a linked list at that location.
//The hashing function used in the course provided crc64.c hash function which takes a string argument and computes a 64-bit hash
//based on cyclic redundancy code computation. Words within files are processed into pairs using the provided getWord.c.

//Usage: wordpairs <-count> fileName1 <fileName2> <fileName3> ...
//Where "count" is the user specified number of wordpairs to return in decreasing order.
//"fileName1" etc are the user selected textfiles to ingest.

#include "functions.h"
#include "crc64.h"
#include "getWord.h"
#include <stdio.h>

int main(int argc, char **argv){

    int hashsize = 50000;
    int tmp;
    char firstchar[]="";
    char extra[257]="";
    char buffer[1024];
    int wordPairsToReturn;
    int argIndex=1;
    int filesToLoad=argc-1;
    int fileIndex=0;
    int hashLoadIncrement=0;
    //char wordPair[257] = "";
    FILE *file[filesToLoad];

    //parsing command line arguments for returned word pair count and files to open for data ingest
    if(argc == 1){
        fprintf( stderr, "Error: no fileName arguments specified.\n\n");
        fprintf( stderr, "\tUsage: ./frequency <-count> fileName1 <fileName2> ...\n\n");
        fprintf( stderr, "\tWhere: count is the number of words to display.\n\n");
        return 1;
    }
    else if(argc == 2){
        if(( sscanf(argv[1], "%c%d%s", firstchar, &tmp, extra) == 2 ) && (argv[1][0]== '-') && (tmp >= 1)){
            fprintf( stderr, "Error: no fileName arguments specified.\n\n");
            fprintf( stderr, "\tUsage: ./frequency <-count> fileName1 <fileName2> ...\n\n");
            fprintf( stderr, "\tWhere: count is the number of words to display.\n\n");
            return 1;
        }
        else if(( sscanf(argv[1], "%c%d%s", firstchar, &tmp, extra) == 2 ) && (argv[1][0] == '-') && (tmp < 1)){
            fprintf( stderr, "Error: Illegal line count argument \'%s\'\n\n", argv[1]);
            fprintf( stderr, "\tUsage: ./frequency <-count> fileName1 <fileName2> ...\n\n");
            fprintf( stderr, "\tWhere: count is the number of words to display.\n\n");
            return 1;
        }
        else{
            // FILE *file[filesToLoad];
            file[argIndex-1] = fopen(argv[argIndex], "r");
            if(file[argIndex - 1] == NULL){
                fprintf( stderr, "Error: Can't read file \'%s\'\n", argv[argIndex]);
                printf("flag1\n");
                return 1;
                
            }
        }
    }
    else if(argc >= 3){

        if(( sscanf(argv[1], "%c%d%s", firstchar, &tmp, extra) == 2) && (argv[1][0] == '-') && (tmp >= 1)){
            wordPairsToReturn = tmp;
            argIndex += 1;
            filesToLoad -= 1;
        }
        else if(( sscanf(argv[1], "%c%d%s", firstchar, &tmp, extra) == 2 ) && (argv[1][0] == '-') && (tmp < 1)){
            fprintf( stderr, "Error: Illegal line count argument \'%s\'\n\n", argv[1]);
            fprintf( stderr, "\tUsage: ./frequency <-count> fileName1 <fileName2> ...\n\n");
            fprintf( stderr, "\tWhere: count is the number of words to display.\n\n");
            return 1;
        }
        // FILE *file[filesToLoad];
        for (argIndex; argIndex < argc; argIndex++){
            file[fileIndex] = fopen(argv[argIndex], "r"); //reading flag
            if(file[fileIndex] == NULL){
                fprintf( stderr, "Error: Can't read file \'%s\'\n", argv[argIndex]);
                return 1;
            }
            fileIndex += 1;
        }
        fileIndex = 0;
    }

    hashEntry *myHash = hashInit(hashsize);

    char wordPair[257] = "";

    for(fileIndex; fileIndex < filesToLoad; fileIndex++){
        //processWordPairs(file[fileIndex]);
        int endOfFileFlag = 0;
        char *currentWord;
        char *previousWord;
        char *tempWord;
        previousWord = getNextWord(file[fileIndex]);
        currentWord = getNextWord(file[fileIndex]);
        while(endOfFileFlag != 1){

            if (previousWord != NULL && currentWord != NULL){
                makeWordPair(previousWord, currentWord, wordPair);
                //hashAdd(myHash, wordPair, hashsize);
                if(hashFind(myHash, wordPair, hashsize) != 1){
                    hashAdd(myHash, wordPair, hashsize);
                    hashLoadIncrement += 1;
                }
                //printf("<%s> ", wordPair);
            }
            if(previousWord == NULL || currentWord == NULL){
                endOfFileFlag = 1;
            }
            tempWord = currentWord;
            free(previousWord);
            currentWord = getNextWord(file[fileIndex]);
            previousWord=tempWord;

        }
        free(previousWord);
    }
    fileIndex = 0;

    for(fileIndex; fileIndex < filesToLoad; fileIndex++){
        fclose(file[fileIndex]);
    }

    int numNodes = hashLoad(myHash, hashsize);
    int arrayIndex = 0;

    struct node *sortingArr[numNodes] = malloc(numNodes * sizeof(struct node));
    
    hashDump(myHash, hashsize, sortingArr);

    hashFree(myHash, hashsize);

    return 0;
}