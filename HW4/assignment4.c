/***********************************************************************
name: John Gregerson
	assignment4 -- acts as a pipe using ":" to seperate programs.
description:	
	See CS 360 Processes and Exec/Pipes lecture for helpful tips.
***********************************************************************/

/* Includes and definitions */
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <errno.h>

/**********************************************************************
./assignment4 <arg1> : <arg2>

    Where: <arg1> and <arg2> are optional parameters that specify the programs
    to be run. If <arg1> is specified but <arg2> is not, then <arg1> should be
    run as though there was not a colon. Same for if <arg2> is specified but
    <arg1> is not.
**********************************************************************/


int main(int argc, char *argv[]){
	/* code */

    //create file descriptors and open a pipe
    int fd[2];
    int rdr, wtr;
    if (pipe(fd) < 0) { //pipe() returns 0 success and -1 on error
        strerror(errno);
        return 1;
    }
    rdr = fd[0]; wtr = fd[1]; // easier to remember names for pipe input/output
    int status;

    //create variables for argv parsing into left and right partitions
    int mode = 0; //tracks if we are parsing args left or right of ':' delimiter
    int leftIndex = 0; //tracks where left args need a null terminator
    int rightIndex = 0; //tracks where right args need a null terminator
    char *leftArgs[argc]; 
    char *rightArgs[argc];
    int hasLeftArgs = 0;
    int hasRightArgs = 0;

    //parse args
    for(int i = 1; i < argc; i++){
        if((*argv[i] == ':') && (mode == 0)){ //switch from reading left args to right args, the mode check is to allow more ':'s in the right args if wanted (ex: echo)
            mode = 1;
        } else if(mode == 0){ //read argv and insert it into an array holding left args
            leftArgs[leftIndex] = argv[i];
            leftIndex++;
            hasLeftArgs = 1;
        } else{ //mode is 1, read argv and insert it into an array holding right args
            rightArgs[rightIndex] = argv[i];
            rightIndex++;
            hasRightArgs = 1;
        }
    }
    
    //null terminate left and right arg arrays.
    leftArgs[leftIndex] = '\0';
    rightArgs[rightIndex] = '\0';

    //determine what partitions need to execute.
    if(hasLeftArgs == 0){ //no left args, only execute right args
        execvp(rightArgs[0], rightArgs);
        
        printf("%s\n", strerror(errno)); //if it got this far execvp failed
        return 1;
    } else if(hasRightArgs == 0){ //no right args, only execute left args
        execvp(leftArgs[0], leftArgs);

        printf("%s\n", strerror(errno)); //if it got this far execvp failed
        return 1;
    } else{
        //create parent/child processes and track each of their process ID's

        int processID = fork(); //returns negative value on failure, zero to child process, positive value to parent process.

        if (processID < 0){ //then fork failed
            printf("%s\n", strerror(errno));
            return 1;
        }
        if (processID == 0){ // then this is the child process
            close(rdr); //close reading end of pipe for child process
            dup2(wtr, 1); //set writer end of pipe to stdout
            close(wtr); //close old wtr

            execvp(leftArgs[0], leftArgs); //execute left args and they will be written to the parent process

            printf("%s\n", strerror(errno)); //if it got this far execvp failed
            return 1;
        } 
        else { //this is the parent process
            wait(&status); //wait for child to end
            close(wtr); //close writing end of pipe
            dup2(rdr, 0); //set reading end of pipe to stdin
            close(rdr); //close old rdr

            execvp(rightArgs[0], rightArgs); //execute right args and they will read input from child process

            printf("%s\n", strerror(errno)); //if it got this far execvp failed
            return 1;
        }
    }

	return 0;
}