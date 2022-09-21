#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "crc64.h"
#include "getWord.h"
#include <limits.h>


//HASH TABLE FUNCTIONS: note crc64.c is course provided and gets its own header file.

//struct node for implementation of hashtable and linked list data structures.
struct node{
  char *pair;
  int occurrences;
  struct node *next;
};

typedef struct node *hashEntry;

hashEntry *hashInit(int hashsize);

int hash(char *wordpair, int hashsize);

void hashAdd(hashEntry *hashTable, char *wordpair, int hashsize);

int hashFind(hashEntry *hashTable, char *wordpair, int hashsize);

int hashLoad(hashEntry *hashTable, int hashsize);

void hashDump(hashEntry *hashTable, int hashsize, struct node *sortingArr);

int hashGrow(hashEntry *oldHashTable, int hashsize); //need to check this declaration later

void hashFree(hashEntry *hashTable, int hashsize);

//List Functions:

struct node *listInit();

void listAdd(struct node *sent, char *wordpair);

int listFind(struct node *sent, char *wordpair);

int listLen(struct node *sent);

void listDump(struct node *sent, struct node *Arr, int *arrayIndex);

void listFree(struct node *sent);

//getWordpairs Functions: note getword.c is course provided and gets its own header file.

void processWordPairs(FILE file[]); //need to check this declaration later

char *makeWordPair(char *firstWord, char *secondWord, char *wordPair);

void printWordPairs(struct node *Arr, int wordPairsToReturn, int numNodes);

int cmpfunc (const void *a, const void *b);


