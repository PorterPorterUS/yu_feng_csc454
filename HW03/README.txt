CSC454 HW03 Yu Feng
NetID: yfeng28

This README includes a biref description about the functionality of my 
major code and how to run and test the code.

First, all the code and running-test is in the same file:
    "translator.ml"

The easiest way to compile and run the test code is using:
        ---- ocaml < translator.ml

By runing this command, you can directly see how I test my code.

This program can automatically general "equivalent" C code, by copy the output 
C code into any text editor, you can generate a very simple program by typing:
        ---- gcc your_code.c -o your_program
and run this program:
        ---- ./your_program

In my demo, the last test is generate a "n_prime" C code, you can directly copy
it can compile that code.

Note that, I didn't record any line from my C code. But any uninitialized
variables will only generate a single warming from the first time it shows
in ocaml AST. 

The general scheme about my code is to follow the example code and implement 
"ast_ize_P" and "translate" this two methods. 

The "ast_ize_P" is just simply follow the match pattern. It will parse the 
PT tree into a AST tree.

The "translate" will follow the AST rule and translate the AST tree, but it
will also record the used variables and generate warnings in the fly.

Please note that, I only implement my code to genrate new line for output C 
code. No indentation in the output.

More information please see code itself.

The Example n_prime.c code also is genrated in the submitted tar.

