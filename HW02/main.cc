//
//  main.cc
//  myParse
//

#include <iostream>
#include "scan.h"
#include "parse.h"

/*
 launch the program and if input has "d", then
 it start the "debug" mode, lots of strings will
 be printed. After the parsing, the program will
 run the script if the script has no error.
 */

int main(int argc, const char * argv[]) {
    bool debug = false;
    if (argc > 1) {
        debug = true;
    }
    myParse* parser = new myParse();
    (*parser).main(debug);
    return 0;
}
