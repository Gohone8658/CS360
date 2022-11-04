//Name: John Gregerson --- Class: CS360 --- Assignment: HW1 --- Date 09/07/2022

//--------------------------------------------------------------------------------------------------------------------------
//Code descriptions are located in function files. (hashtable.c and getwordPairs.c) README information is also located in functions.h and main.c
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
//usually range from 1 to MAX_INT <typically 2147483647> with a default of 1000. values over 250 million begins
//to take a long time to compile and result in the program being killed.

//----------------------------------------------------------------------------------------------------------------------------