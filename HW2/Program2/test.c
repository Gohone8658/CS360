#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h> //remove this
#include <stdlib.h>

int main(void){

    char *word = "dog";
    int length = 9;
    char wordTrunc[length+1];
    int i = 0;

    while(word[i] != '\0'){
        wordTrunc[i] = word[i];
        i++;
    }
    while(i <= length-2 ){
        if(i <= length - 2){
            wordTrunc[i] = '.';
            i++;
        }
    }
    wordTrunc[length -1] = '\n';
    wordTrunc[length]='\0';
	printf("<%s>", wordTrunc);

    return 0;

}