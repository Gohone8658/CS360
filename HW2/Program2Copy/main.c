#include <stdio.h>

int lineNum(char *dictionaryName, char *word, int length);

int main(void){

    //printf("before function\n");

    int result = 0;
    result = lineNum("webster_16", "delimit", 16);
    printf("line number %d\n",  result);

    //printf("after function\n");

    return 0;

}

