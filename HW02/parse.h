#ifndef MYPARSER_H
#define MYPARSER_H

#include "scan.h"
#include <map>
#include <set>

using namespace std;

const string names[] = {"read", "write", "id", "num", ":=",
            "+", "-", "*", "/", "(", ")", "eof",
            "if", "fi", "do", "od", "check", "==", "<>", "<",
            ">", "<=", ">="
};

class treeNode {
public:
    treeNode(string v, treeNode* l, treeNode* r) {
        value = v;
        left = l;
        right = r;
    }
    string value;
    treeNode* left;
    treeNode* right;
};

static token input_token;
static map<string, set<string>>first_map;
static map<string, set<string>> follow_map;
static set<string> eps = {"SL", "ET", "TT", "FT"};
static map<string, set<string>> predict_map;

static treeNode* root ;
static int indentation;
static string output_string;

class myParse
{
public:
    myParse();
    ~myParse();

    static int main();

private:

    static void program();
    static treeNode* stmt_list();
    static treeNode* stmt();
    static treeNode* relat();
    static treeNode* expr();
    static treeNode* expr_tail(treeNode *r);
    static treeNode* term();
    static treeNode* term_tail(treeNode *e);
    static treeNode* factor();
    static treeNode* factor_tail(treeNode *t);
    static string add_op();
    static string mul_op();
    static string ro_op();

    static void addIndent();
    static void addSpace();
    static void addReturn();
    
    static void error();
    static void init();
    static void init_first();
    static void init_follow();
    static void init_predict();
    
    static string match(token expected);
    
};


#endif
