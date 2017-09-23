#ifndef MYPARSER_H
#define MYPARSER_H

#include "scan.h"
#include <map>
#include <set>

using namespace std;

const string names[] = {"read", "write", "id", "lit", ":=",
    "add", "sub", "mul", "div", "lparen", "rparen", "eof",
    "if", "fi", "do", "od", "check", "==", "<>", "<",
    ">", "<=", ">="
};

static token input_token;
static map<string, set<string>>first_map;
static map<string, set<string>> follow_map;
static set<string> eps = {"SL", "ET", "TT", "FT"};

static map<string, set<string>> predict_map;

class myParse
{
public:
    myParse();
    ~myParse();

    static int main();

private:
    static void program();
    static void stmt_list();
    static void stmt();
    static void relat();
    static void expr();
    static void expr_tail();
    static void term();
    static void term_tail();
    static void factor();
    static void factor_tail();
    static void add_op();
    static void mul_op();
    static void ro_op();

    static void error();
    static void init();
    static void init_first();
    static void init_follow();
    static void init_predict();
    
    static void match(token expected);
    
};


#endif
