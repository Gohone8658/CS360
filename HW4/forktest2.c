#include <unistd.h>
#include <stdio.h>
#include <math.h>
#include <sys/wait.h>

void exit(int status);

int main (int argc, char *argv[]) {
// char *who;
// int status;
// if (fork ()) {
// who = "parent";
// printf ("pi=%f\n", 4*atan(1));
// wait (&status);
// exit (0);
// } else {
// who = "child";
// execlp ("/usr/bin/date", "date", (char *)0);
// execlp ("/usr/bin/date", "date", (char *)0);
// //close(1);
// }

printf ("a");
if (fork ()) {
printf ("b");
} else {
printf ("c");
}
exit (0);
}

    // printf("left args = ");
    // for(int j = 0; j <= leftIndex; j++){
    //     printf("%s ", leftArgs[j]);
    // }
    // printf("\nright args = ");
    // for(int k = 0; k <= rightIndex; k++){
    //     printf("%s ", rightArgs[k]);
    // }
    // printf("\n");

    // //char *arr[] = {"ls", "-la", NULL};
    // //execvp(leftArgs[0], leftArgs);
    // execvp(rightArgs[0], rightArgs);