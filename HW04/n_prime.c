#include "n_prime.h"


void putint(int n) {
    printf("%d\n", n);
}


void main() {
    int n = getint();
    int a_long_calc = 100 / (9 + 3) + 5 * 7 + 100000 * 
                   5 * 6 / 4 + 
                   4 / 6 * 4 + 
                   2 + 2 + 1;
    putint(a_long_calc);
    for (int i = 0; i < n; i++) {
        putint(n);
    }
    
}