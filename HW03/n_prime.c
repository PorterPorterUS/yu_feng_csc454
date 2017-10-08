#include <stdio.h>
#include <stdlib.h>

int getint() {
    int c, num;
    num = 0;
     
    while(isspace(c = getchar()));
     
    if (!isdigit(c)) {
        fprintf(stderr, "This is not a valid character: %c.\n", c);
        fprintf(stderr, "Please enter only a positive integer.\n");
        exit(-1);
    }
    
    while(isdigit(c)) {
        num = (num * 10) + (c - '0');
        c = getchar();
    }
    return c;
}

void putint(int n) {
  printf("%d\n", n);
}