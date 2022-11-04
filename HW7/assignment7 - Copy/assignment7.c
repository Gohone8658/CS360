#include "assignment7.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>
#include <stdio.h>
//#include <unistd.h>

//Name: John Gregerson --- Course: CS360 --- Assignment: Program 7 --- Date 10/31/2022
//Synopsis: This program will build upon the provided quicksort function and speed it up by implementing multi-threading.
//Maximum threads is user defined. Input will be an array of strings and Output will be a sorted list of those strings.

//NOTES: Due to current workload I am going to keep my current implementation instead of working on a pooled threads implementation.
//Current implementation will create two threads per iteration assuming max number of threads has not been reached.
//I can think of a way to implement a thread pool through the creation of a stack used to track sort partition indexes but would
//require significant reworking to how quicksort and insertSort are implemented. This could be done through making the sortParams
//struct (left and right) fields into integer arrays and setting the positions of the left and right indexes appropriately.
//incrementing the index whenever the range is large enough to use quick sort and decremening when insertSort is used.
//The main thread would be split from in sortThreaded() executing quickSort(), quicksort would then create waiting threads until currentThreads = maximumThreads

#define SORT_THRESHOLD      40

typedef struct _sortParams {
    char** array;
    int left;
    int right;
} SortParams;

static int maximumThreads;              /* maximum # of threads to be used */
int currentThreads=0;
pthread_mutex_t mutex;  //create mutex for thread count increment

/* This is an implementation of insert sort, which although it is */
/* n-squared, is faster at sorting short lists than quick sort,   */
/* due to its lack of recursive procedure call overhead.          */

static void insertSort(char** array, int left, int right) {
    int i, j;
    for (i = left + 1; i <= right; i++) {
        char* pivot = array[i];
        j = i - 1;
        while (j >= left && (strcmp(array[j],pivot) > 0)) {
            array[j + 1] = array[j];
            j--;
        }
        array[j + 1] = pivot;
    }
}

/* Recursive quick sort, but with a provision to use */
/* insert sort when the range gets small.            */

static void quickSort(void* p) {
    SortParams* params = (SortParams*) p;
    char** array = params->array;
    int left = params->left;
    int right = params->right;
    int i = left, j = right;
    
    if (j - i > SORT_THRESHOLD) {           /* if the sort range is substantial, use quick sort */

        int m = (i + j) >> 1;               /* pick pivot as median of         */
        char* temp, *pivot;                 /* first, last and middle elements */
        if (strcmp(array[i],array[m]) > 0) {
            temp = array[i]; array[i] = array[m]; array[m] = temp;
        }
        if (strcmp(array[m],array[j]) > 0) {
            temp = array[m]; array[m] = array[j]; array[j] = temp;
            if (strcmp(array[i],array[m]) > 0) {
                temp = array[i]; array[i] = array[m]; array[m] = temp;
            }
        }
        pivot = array[m];

        for (;;) {
            while (strcmp(array[i],pivot) < 0) i++; /* move i down to first element greater than or equal to pivot */
            while (strcmp(array[j],pivot) > 0) j--; /* move j up to first element less than or equal to pivot      */
            if (i < j) {
                char* temp = array[i];      /* if i and j have not passed each other */
                array[i++] = array[j];      /* swap their respective elements and    */
                array[j--] = temp;          /* advance both i and j                  */
            } else if (i == j) {
                i++; j--;
            } else break;                   /* if i > j, this partitioning is done  */
        }
        
        //thread variables for left and right splits
        pthread_t leftThread;
        pthread_t rightThread;
        int createdLeft=0;
        int createdRight=0;
        int threadCheck=0; //error checking variable

        SortParams first;  first.array = array; first.left = left; first.right = j;
        SortParams second; second.array = array; second.left = i; second.right = right;
        //quickSort(&first);                  /* sort the left partition  */
        if(currentThreads < maximumThreads){ //make new thread left
            pthread_mutex_lock(&mutex);
            // if(threadCheck != 0){
            //     fprintf(stderr, "%s\n", strerror(errno));
			//     //return;
            // }
            threadCheck = pthread_create(&leftThread, NULL, (void *)quickSort, &first);
            if(threadCheck != 0){
                fprintf(stderr, "%s\n", strerror(errno));
			    //return;
            }
            currentThreads++;
            createdLeft = 1;
            pthread_mutex_unlock(&mutex);
        } else{
            quickSort(&first);
        }

        //quickSort(&second);                 /* sort the right partition */
        if(currentThreads < maximumThreads){ //make new thread right
            pthread_mutex_lock(&mutex);
            threadCheck = pthread_create(&rightThread, NULL, (void *)quickSort, &second);
            if(threadCheck != 0){
                fprintf(stderr, "%s\n", strerror(errno));
			    //return;
            }
            currentThreads++;
            createdRight = 1;
            pthread_mutex_unlock(&mutex);
        } else{
            quickSort(&second);
        }

        if(createdLeft == 1){ //have to wait for it to finish
            pthread_join(leftThread, NULL);
        }
        if(createdRight == 1){ //have to wait for it to finish
            pthread_join(rightThread, NULL);
        }
                
    } else insertSort(array,i,j);           /* for a small range use insert sort */

}

/* user interface routine to set the number of threads sortT is permitted to use */

void setSortThreads(int count) {
    maximumThreads = count;
}

/* user callable sort procedure, sorts array of count strings, beginning at address array */

void sortThreaded(char** array, unsigned int count) {
    SortParams parameters;
    int mutexCheck;
    parameters.array = array; parameters.left = 0; parameters.right = count - 1;
    mutexCheck = pthread_mutex_init(&mutex, NULL);
    if(mutexCheck != 0){
        fprintf(stderr, "%s\n", strerror(errno));
        return;
    }
    quickSort(&parameters);
    pthread_mutex_destroy(&mutex);
}