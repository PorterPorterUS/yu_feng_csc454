//
//  main.cpp
//  myParse
//
//  Created by AllenFeng on 9/22/17.
//  Copyright © 2017 FY. All rights reserved.
//

#include <iostream>
#include "scan.h"
#include "parse.h"


int main(int argc, const char * argv[]) {
    bool debug = false;
    if (argc > 1 && *(argv[1]) == 'd') {
        debug = true;
    }
    myParse* parser = new myParse();
    (*parser).main(debug);
    return 0;
}
