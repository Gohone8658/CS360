#include "functions.h"

//allocates memory for entries in the hashtable, each large enough for a struct node.
//iterates through each hash entry and creates a sentinel node to begin a linked list using listInit()
hashEntry *hashInit(int hashsize){
    hashEntry *hashTable = malloc(sizeof(hashEntry)*hashsize);
    for(int i=0; i < hashsize; i++){
        hashTable[i] = listInit();
    }
    return(hashTable);
}

//this function returns the position value of the input wordpair in the hash array after hashing through the crc64 function.
int hash(char *wordpair, int hashsize){
    unsigned long long val = 0;
    val = crc64(wordpair);
    val = val % hashsize;
    return(val);
}

//hashes a wordpair for the correct sentinel node location in the hash array and passes both to listAdd().
void hashAdd(hashEntry *hashTable, char *wordpair, int hashsize){
    listAdd(hashTable[hash(wordpair, hashsize)], wordpair);
}

//hashes a wordpair to pass to listFind() the sentinel node location for that pair in the hash array.
//returns 1 if the wordpair was found and 0 otherwise.
int hashFind(hashEntry *hashTable, char *wordpair, int hashsize){
    int result = listFind(hashTable[hash(wordpair, hashsize)], wordpair);
    return result;
}

//Iterates through each sentinel node in the hash table calling listLen() to count
//and sum total nodes in the hash table.
int hashLoad(hashEntry *hashTable, int hashsize){
    int numNodes = 0;
    int listNodes = 0;
    //struct node *current;
    for(int i = 0; i < hashsize; i++){
        listNodes = listLen(hashTable[i]);
        numNodes += listNodes;       
    }
    return(numNodes);
}

//Iterates through each sentinel node in the hash table calling listDump() to add thier node
//to the struct array. Keeps track of what position of the array we are at while passing to listDump
//for use with qsort and printWordPairs().
void hashDump(hashEntry *hashTable, int hashsize, struct node *sortingArr){
    int *arrayIndex;
    int a = 0;
    arrayIndex = &a;
    int i = 0;
    for(int i = 0; i < hashsize; i++){
        listDump(hashTable[i], sortingArr, arrayIndex);
    }

}

//Creates a new hashTable with a magnitude of 3 larger hashsize 
//moves over previous hashtable before freeing previous hashtable.
//returns new hashSize for being called at a later point.
int hashGrow(hashEntry *oldHashTable, int hashsize){
    int newHashSize = hashsize * 3;
    hashEntry *newHashTable = hashInit(newHashSize);

    for(int i = 0; i < hashsize; i++){
        struct node *current;
        current = oldHashTable[i]->next;

        while(current != NULL){
            if((current->occurrences) != 0){
                current->occurrences++;
                return 1;
            }
        current = current->next;
        }
    }
}

//cleanup function, iterates through the hash array calling the listFree function for each sentinel node.
//list free cleans each linked list.
//hash array itself is then cleaned from memory

void hashFree(hashEntry *hashTable, int hashsize){
    for(int i = 0; i < hashsize; i++){
        listFree(hashTable[i]);
    }
    free(hashTable);
    return;
}

//initialized linked list sentinel node
struct node *listInit(){
    struct node *temp=malloc(sizeof(struct node));
    temp->pair=NULL;
    temp->occurrences=0;
    temp->next=NULL;
    return(temp);
}

//adds a new node to the front of a linked list at the passed hash location.
//each node datafield is malloc'd it's own space
//tailored to the length of the passed variable.
//*newOccurrences is redundant but adds clarity for me.

void listAdd(struct node *sent, char *pair){
    struct node *new = malloc(sizeof(struct node));
    char *newPair = malloc((strlen(pair)+1));
    strcpy(newPair, pair);

    int newOccurrences = 1;

    new->pair=newPair;
    new->occurrences=newOccurrences;
    new->next=sent->next;
    sent->next=new;
}

//takes in a hash location and iterates through it's linked list,
//if a node with the same wordpair is found increments occurrences and returns 1.
int listFind(struct node *sent, char *wordpair){
    struct node *current;
    current = sent->next;

    while(current != NULL){
        if(strcmp(wordpair, current->pair) == 0){
            current->occurrences +=1;
            return 1;
        }
        current = current->next;
    }
    return 0;
}

//iterates through a linked list sent from hashLoad and sums the number of non-sentinel nodes present.
//returns them to hashLoad.
int listLen(struct node *sent){
    struct node *current = sent->next;
    int listNodes = 0;
    while(current != NULL){
        current = current->next;
        listNodes += 1;
    }
    return(listNodes);
}

//iterates through a linked list handed to it by HashDump and increments
//an array counter. This array counter is not reset as listDump is called multiple
//times and the array being filled is larger than any single linked list.
void listDump(struct node *sent, struct node *Arr, int *arrayIndex){
    struct node *current = sent->next;

    while(current != NULL){
        Arr[*arrayIndex] = *current;
        current = current->next;
        (*arrayIndex)++;
    }
}

//cleanup function, steps through a linked list passed from the hash array
//freeing memory for each malloc'd data field.
void listFree(struct node *sent){
    struct node *current;
    struct node *previous;
    struct node *temp;

    current=sent->next;
    previous=sent;

    while(current != NULL){
        temp=previous->next;
        free(previous->pair);
        //free(previous->occurrences);
        free(previous);
        current=current->next;
        previous=temp;
    }
    free(previous->pair);
    //free(previous->occurrences);
    free(previous);
    return;
}

//Compare function for use with qsort()
int cmpfunc (const void *a, const void *b){
    struct node *nodeA = (struct node*)a;
    struct node *nodeB = (struct node*)b;
    return(nodeB->occurrences - nodeA->occurrences);
}