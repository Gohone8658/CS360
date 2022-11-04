//Name: John Gregerson --- Class: CS360 --- Assignment: HW1 --- Date 09/07/2022

//Code descriptions are located in function files. (hashtable.c and getwordPairs.c) with the same information in functions.h

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

//note: initial hashsize can be changed by adjusting the "hashsize" variable. allowable variables
//usually range from 1 to MAX_INT <typically 2147483647>. values over 250 million begins
//to take a long time to compile and result in the program being killed.

#include "functions.h"
#include "crc64.h"
#include "getWord.h"
#include <stdio.h>

int main(int argc, char **argv){

    long long int hashsize = 10000;
    int tmp;
    char firstchar[]="";
    char extra[257]="";
    char buffer[1024];
    int wordPairsToReturn = 0;
    int argIndex=1;
    int filesToLoad=argc-1;
    int fileIndex=0;
    int HASH_MAX = 200000000;

    FILE *file[filesToLoad];

    if(hashsize > HASH_MAX || hashsize < 0){
        fprintf( stderr, "Error: hash size must be greater than zero and less than %d.\n\n", HASH_MAX);
        fprintf( stderr, "\tPlease set hashsize in the main.c file to an acceptable value.\n\n");
        return 1;
    }

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

        for (argIndex; argIndex < argc; argIndex++){
            file[fileIndex] = fopen(argv[argIndex], "r"); //reading flag
            if(file[fileIndex] == NULL){
                fprintf( stderr, "Error: Can't read file \'%s\'\n", argv[argIndex]);
                return 1;
            }
            fileIndex += 1;
        }
        fileIndex = 0; //reset fileIndex for "Opening filestreams for-loop"
    }

    hashEntry *myHash = hashInit(hashsize);

    char wordPair[257] = "";
    int hashLoadIncrement=0; //tracks nodes to determine load factor.

    //iterate through each file calling with getNextWord and adding to hash or incrementing
    //a tempWord is kept to keep free() from freeing words still in use.
    //previousWord and currentWord move in lock step to each make up one half of a word pair.
    for(fileIndex; fileIndex < filesToLoad; fileIndex++){
        int endOfFileFlag = 0;
        char *currentWord;
        char *previousWord;
        char *tempWord;
        previousWord = getNextWord(file[fileIndex]);
        currentWord = getNextWord(file[fileIndex]);
        while(endOfFileFlag != 1){
            if (previousWord != NULL && currentWord != NULL){
                makeWordPair(previousWord, currentWord, wordPair);
                if(hashFind(myHash, wordPair, hashsize) != 1){
                    hashAdd(myHash, wordPair, hashsize, 0);
                    hashLoadIncrement += 1;
                    if(hashLoadIncrement == hashsize){
                        //printf("<%lld>", hashsize); //these were used to check taht the hash size was growing.
                        hashGrow(&myHash, &hashsize);
                        //printf("<%lld>", hashsize);
                    }
                }
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
    fileIndex = 0; //reset fileIndex for "closing filestreams for-loop"

    for(fileIndex; fileIndex < filesToLoad; fileIndex++){
        fclose(file[fileIndex]);
    }

    int numNodes = hashLoad(myHash, hashsize);
    struct node *sortingArr = malloc(numNodes * sizeof(struct node));
    
    //hashDump will create the array for qsort() (see hashtable.c for description.)
    hashDump(myHash, hashsize, sortingArr);

    //qsort will format the array for printing (see hashtable.c for description.)
    qsort(sortingArr, numNodes, sizeof(struct node), cmpfunc);

    printWordPairs(sortingArr, wordPairsToReturn, numNodes);

    free(sortingArr);
    hashFree(myHash, hashsize);

    return 0;
}