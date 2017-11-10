#include "n_prime.h"
/* Comments!! */
/* Comments */
int Glo_num = 13;

/* Comments before putint */
void putint(int n) {
    /* Comments: in putint */
    /* Comments: before printf */
    printf("%d\n", n);
    /* Comments after printf */
}
/* Comments after putint!! */

/* Comments before main!! */
void main() {
    /* Comments in main!! */
    int n = getint();
    /* Comments before long calc!! */
    int a_long_calc = 100 / (9 + 3) + 5 * 7 + 100000 * 
                   5 * 6 / 4 + 
                   4 / 6 * 4 + 
                   2 + 2 + 1;
    /* Comments after calc!! */
    putint(a_long_calc);
    /* Comments before loop */
    for (int i = 0; i < n; i++) {
        /* Comments in loop */
        putint(n);
    }    
    /* Comments after loop */
}
/* Comments after main */