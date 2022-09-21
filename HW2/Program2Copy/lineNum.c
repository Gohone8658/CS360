/***********************************************************************
name: John Gregerson
	lineNum -- see if a word is in the online dictionary
description:	
	See CS 360 IO lecture for details.
***********************************************************************/

/* Includes and definitions */
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

/**********************************************************************
Search for given word in dictionary using file descriptor fd.
Return the line number the word was found on, negative of the last line searched
if not found, or the error number if an error occurs.
**********************************************************************/

int lineNum(char *dictionaryName, char *word, int length) {

	int errorPrint(); //manually prints errno and strerror to stderror using the write() function.
	
	//return value and file descriptor for given dictionary.
	int retVal;
	int fd;

	//holds lseek offsets for binary search.
	int offsetLow;
	int offsetHigh;
	int offsetCurrent;

	//format and truncate 'word' to match dictionary format, NULL terminator is required for strcmp().
	char wordTrunc[length+1]; //length+1 to hold NULL terminator.
	int i = 0;

	//copy characters from word into wordTrunc.
	while(word[i] != '\0'){
        wordTrunc[i] = word[i];
        i++;
    }
	//fill the rest of the space of wordTrunc with whitespace.
    while(i <= length-2 ){
        if(i <= length - 2){
            wordTrunc[i] = ' ';
            i++;
        }
    }
    wordTrunc[length -1] = '\n';
    wordTrunc[length]='\0';	

	//open file for reading then error check.
	fd = open(dictionaryName, O_RDONLY);
	if(errno != 0) errorPrint(errno);

	//set byte offsets for binary search.
	offsetLow = lseek(fd, 0, SEEK_SET); //grabs current file offset, which should be zero on a newly opened file.
	offsetHigh = lseek(fd, 0, SEEK_END); //grabs offset of end of file.

	//binary search loop
	while ((offsetLow != offsetHigh) && (offsetLow < offsetHigh)){
		char buffer[length + 1]; //length+1 for new line character and null terminator necessary for strcmp()
		offsetCurrent = (((offsetLow + offsetHigh) / 2) / length) * length; // divide and multiply by length to remove the remainder so offsetCurret begins at the start of a line.
		
		lseek(fd, offsetCurrent, SEEK_SET);
		read(fd, buffer, length);
		if(errno != 0) errorPrint(errno);
		buffer[length] = '\0';

		if(strcmp(wordTrunc, buffer) == 0){ //input has been found return line number.
			close(fd);
			retVal = (offsetCurrent / length) + 1; // offset begins at zero, where the dictionary line would be 1.
			return (retVal);
		}
		else if((strcmp(wordTrunc, buffer) > 0) && (offsetLow != (offsetHigh - length))){ //input is greater in value than our current search.
			offsetLow = offsetCurrent;
		}
		else if((strcmp(wordTrunc, buffer) < 0) && (offsetLow != (offsetHigh - length))){ //input is lesser in value than our current search.
			offsetHigh = offsetCurrent;
		}
		else{
			offsetLow += length; // last possible line was searched, increments offsetLow to meet exit condition.
		}
	}
	close(fd);
	retVal = (offsetCurrent / length +1); //offset begins at zero, where the dictionary line would be 1.
	retVal = retVal - (2 * retVal); //sets retVal to the negative of itself.

	if(errno != 0) errorPrint(errno);

	return (retVal);
}

int errorPrint(){
	char errnoAsString[512];
	char tmpStrerror[512];
	int strerrorLength = sprintf(tmpStrerror, "%s", strerror(errno)); //Ben used this as an approach in the CS360 slack.
	int errnoLength = sprintf(errnoAsString, "%d", errno);

	write(2, "errno: ", 7);
	write(2, errnoAsString, errnoLength);
	write(2, "\nstrerror message: ", 19);
	write(2, strerror(errno), strlen(strerror(errno)));

	return(errno);
}

//If I were to start this assignment again I would turn error printing into a function
//and use the error printing function after each read/write/open/close call to return errors.
//I did actually implement this after initial submission but it would print multiple errors because it did not exit after printing.