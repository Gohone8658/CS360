#include <unistd.h>
#include <stdio.h>

void exit(int status);

int main (int argc, char *argv[]) {
    char *who;
    int i;
    if (fork ()) {
        who = "parent";
    } else {
        who = "child";
    }
    for (i = 0; i < 6; i++) {
        printf ("*fork1: %s\n", who);
    }
    exit (0);
}