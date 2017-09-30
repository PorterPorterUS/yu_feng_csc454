# yu_feng_csc454_hw02
# This is directory is for HW02 in csc 454.

To compile this program:
-- g++ -o main main.cc parse.cc scan.cc

To run this program:
-- ./main (d) (< sample.txt)
-- d: debug mode. it will help to see how 
    the code work during the process and
    easy to debug.
-- sample.txt: it can be a correct input and 
    it will help to run the code and generate 
    right syntax tree output as it seen on the 
    website, and then excute the code based on 
    the input.

The structure of directory is listed below:
-- README.md   
-- main        
-- main.cc     
-- parse.cc    
-- parse.h     
-- sample.txt  
-- scan.cc     
-- scan.h

main.cc:
    it is a main function to call the parser class
    and if the main with "d", it will print infor-
    mation for debuging.

parse.h and parse.cc
    they are the main class for parsing and examing 
    the correctness of input syntax, it is based on
    context-specific error detection, it will shoot 
    the error right way, and then, the stmt, expr or
    relation will catch the error and print where the 
    error is come from and some error message, simply:
    print the current bugged line and its line number 
    and column index. more details please see the code 
    itself.
    If the input is grammer-correct, it will generate
    the syntax tree (like the one on the website) and 
    it will excute the code based on the input string.
    run "./main < sample.txt" can check out the number
    of prime function.
    In the parse.cc, I actually write down the general
    FIRST, FOLLOW, and PREDICT sets, and group them in
    a map. So, it is relatively easy to pass them around
    the code. The context-specific FOLLOW sets are generated
    "on the fly".    

scan.cc and scan.h:
    it is simple scanner, based on the previous C code,
    it added some more condition to recognize the rela-
    tion operators and "$$", just easy for user to input
    some instructions on the console and easy for me to
    debug.









