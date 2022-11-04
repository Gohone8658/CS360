/***********************************************************************
name: John Gregerson
	readable -- recursively count readable files.
description:	
	See CS 360 Files and Directories lectures for details.
***********************************************************************/

/* Includes and definitions */

/**********************************************************************
Take as input some path. Recursively count the files that are readable.
If a file is not readable, do not count it. Return the negative value
of any error codes you may get. However, consider what errors you want
to respond to.
**********************************************************************/
#include <linux/limits.h>
#include <errno.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include <string.h>
#include <stdio.h>

int readable(char *inputPath) {
	//This is where you do the work. ;)
	char currentPath[PATH_MAX];
	char *currentFile;
	int i = 0;
	int fileCount = 0; // readable files

	DIR *dir;
	struct dirent *dp;

	struct stat area, *s = &area;

	//set currentPath to inputPath

	if(inputPath == NULL || inputPath[0] == '\0'){ //no path specified
		getcwd(currentPath, PATH_MAX);
	}
	else if(inputPath[0] == '/'){ //absolute path specified
		while(inputPath[i] != '\0'){
        	currentPath[i] = inputPath[i];
        	i++;
    	}
		currentPath[i] = '\0';
	}
	else if(inputPath[0] != '/' && inputPath[0] != '\0' && inputPath != NULL){ //relative path specified have to add "./" does not effect './' or '../' if present.
		currentPath[i] = '.';
		i++;
		currentPath[i] = '/';
		i++;
		while(inputPath[i-2] != '\0'){
        	currentPath[i] = inputPath[i-2];
        	i++;
    	}
		currentPath[i] = inputPath[i-2];
		currentPath[i+1] = '\0';
	}

	if (lstat (currentPath, s) == 0){ //path exists and lstat returned successfully
		if (S_ISREG (s->st_mode)){ //regular file
			int result = access (currentPath, R_OK); // Catches access permissions if it is the first file called
    		if ( result == 0 ) {
				fileCount++;
    		} 
			else { //access denied on first file return errno as a negative.
				char errnoAsString[512];
				char tmpStrerror[512];
				int strerrorLength = sprintf(tmpStrerror, "%s", strerror(errno)); //Ben used this as an approach in the CS360 slack.
				int errnoLength = sprintf(errnoAsString, "%d", errno);

				write(2, "errno: ", 7);
				write(2, errnoAsString, errnoLength);
				write(2, "\nstrerror message: ", 19);
				write(2, strerror(errno), strlen(strerror(errno)));

				return(errno - 2*errno); //converts errno to it's negative  
    		}
		}
		else if(S_ISDIR (s->st_mode)){ //directory to process
			int result = access (currentPath, R_OK); // Catches access permissions if it is the first file called
    		if ( result == 0 ) {
				dir = opendir(currentPath); //open currently examined directory.
				chdir(currentPath); //step into directory for current to next relative call

				while ((dp = readdir(dir)) != NULL){ //iterate through each entry in the dir
					currentFile = dp->d_name;
					if(strcmp(currentFile, "..") != 0 && strcmp(currentFile, ".") != 0){ //ignore current and above directories
						if(access(currentFile, R_OK) == 0){ //check permissions on potential next file
							fileCount = fileCount + readable(currentFile); //recursively call this function with a relative path to entry, set filecount equal to the sum of all calls.
						}
					}
				}
				chdir(".."); //step back out of directory, important for keeping relative paths with multiple directories.
				closedir(dir); //close currently examined directory (this will work back up the ladder closing all that have been opened).
			}
			else { //access denied on first directory return errno as a negative.
				char errnoAsString[512];
				char tmpStrerror[512];
				int strerrorLength = sprintf(tmpStrerror, "%s", strerror(errno)); //Ben used this as an approach in the CS360 slack.
				int errnoLength = sprintf(errnoAsString, "%d", errno);

				write(2, "errno: ", 7);
				write(2, errnoAsString, errnoLength);
				write(2, "\nstrerror message: ", 19);
				write(2, strerror(errno), strlen(strerror(errno)));

				return(errno - 2*errno); //converts errno to it's negative  
    		}
		}
    	return (fileCount); //returns to most previous call, will equal 1 in regular files and the sum of below calls if directory.
	} 
	else{ //lstat failed at some point return errno as a negative. should only happen on first passed path.
		char errnoAsString[512];
		char tmpStrerror[512];
		int strerrorLength = sprintf(tmpStrerror, "%s", strerror(errno)); //Ben used this as an approach in the CS360 slack.
		int errnoLength = sprintf(errnoAsString, "%d", errno);

		write(2, "errno: ", 7);
		write(2, errnoAsString, errnoLength);
		write(2, "\nstrerror message: ", 19);
		write(2, strerror(errno), strlen(strerror(errno)));

		return(errno - 2*errno); //converts errno to it's negative
	}
}
