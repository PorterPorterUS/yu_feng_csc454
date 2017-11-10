#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
int getint() {
    int num, nitems;
    nitems = scanf("%d", &num);
    return num;
}

void putint(int n) {
    printf("%d\n", n);
}