#include <stdio.h>

#include <stdlib.h>

#include <ctype.h>



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

void main() {
int n = getint();
int cp = 2;
while (1){
if (!( n>0 )) break;
int found = 0;
int cf1 = 2;
int cf1s = cf1*cf1;
while (1){
if (!( cf1s<=cp )) break;
int cf2 = 2;
int pr = cf1*cf2;
while (1){
if (!( pr<=cp )) break;
if (pr==cp) {
found = 1;
}
cf2 = cf2+1;
pr = cf1*cf2;
}
cf1 = cf1+1;
cf1s = cf1*cf1;
}
if (found==0) {
putint( cp );
n = n-1;
}
cp = cp+1;
}
}