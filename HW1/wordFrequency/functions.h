#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include "crc64.h"
#include "getWord.h"
#include <limits.h>
//Name: John Gregerson --- Class: CS360 --- Assignment: HW1 --- Date 09/07/2022

//--------------------------------------------------------------------------------------------------------------------------
//Code descriptions are located in function files. (hashtable.c and getwordPairs.c) with the same information in functions.h
//--------------------------------------------------------------------------------------------------------------------------
//program files context: (crc64.c, crc64.h, getWord.c, and getWord.h) are course provided and implement a hash algorithm
//and getword function respectively. (functions.h) includes the function definitions for both getwordpairs.c and hashtable.c
//(getwordpairs.c) contains functions for parsing ingested text into wordpairs. (hashtable.c) contains functions for
//the initialization and interaction with a linked list implemented hashtable.

//This program will allow the user to read in words from one or multiple files and
//output a list of a user specified number of most frequently appearing sequential pairs of words.

//Implemented via a hasharray assigning each location in the array to a sentinel node to begin a linked list at that location.
//The hashing function used in the course provided crc64.c hash function which takes a string argument and computes a 64-bit hash
//based on cyclic redundancy code computation. Words within files are processed into pairs using the provided getWord.c.

//USAGE: wordpairs <-count> fileName1 <fileName2> <fileName3> ...
//Where "count" is the user specified number of wordpairs to return in decreasing order.
//"fileName1" etc are the user selected textfiles to ingest.

//note: initial hashsize can be changed by adjusting the "hashsize" variable in main.c. allowable variables
//usually range from 1 to MAX_INT <typically 2147483647>. values over 250 million begins
//to take a long time to compile and result in the program being killed.

//-----------------------------------------------------------------------------------------------------------------------------------------------------
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

void hashAdd(hashEntry *hashTable, char *wordpair, int hashsize,int occurrences);

int hashFind(hashEntry *hashTable, char *wordpair, int hashsize);

int hashLoad(hashEntry *hashTable, int hashsize);

void hashDump(hashEntry *hashTable, int hashsize, struct node *sortingArr);

void hashGrow(hashEntry **hashTable, long long int *hashsize);

void hashFree(hashEntry *hashTable, int hashsize);

//List Functions:

struct node *listInit();

void listAdd(struct node *sent, char *wordpair, int occurrences);

int listFind(struct node *sent, char *wordpair);

int listLen(struct node *sent);

void listDump(struct node *sent, struct node *Arr, int *arrayIndex);

void listFree(struct node *sent);

//getWordpairs Functions: note getword.c is course provided and gets its own header file.

char *makeWordPair(char *firstWord, char *secondWord, char *wordPair);

void printWordPairs(struct node *Arr, int wordPairsToReturn, int numNodes);

int cmpfunc (const void *a, const void *b);


