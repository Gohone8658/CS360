#include <stdio.h>
#include <linux/limits.h>
#include <unistd.h>
int readable(char *inputPath);

int main(void){

    //printf("before function\n");

    int result = 0;
    result = readable(NULL);
    printf("\nreadable files: %d\n",  result);

    //printf("after function\n");

    return 0;

}