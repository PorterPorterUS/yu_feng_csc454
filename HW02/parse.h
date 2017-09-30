#ifndef MYPARSER_H
#define MYPARSER_H

#include "scan.h"
#include <map>
#include <set>
#include <regex>

using namespace std;

const string names[] = {"read", "write", "id", "num", ":=",
            "+", "-", "*", "/", "(", ")", "$$",
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


class myParse
{
public:
    myParse();
    ~myParse();
    int main(bool debug);
    void printTree(treeNode *t);

private:
    /*
     the scanner instance for parser.
     */
    myScan* scanner;
    /*
     current input_token.
     */
    token input_token;
    /*
     if isError is true, the program will stop to
     print the syntax tree.
     */
    bool isError = false;
    /*
     ifDebug mode is on, the program will print the
     state at each function call.
     */
    bool ifDebug = false;
    /*
     tree root: easy to print the syntax tree.
     */
    treeNode* root;
    /*
     output string is only used for print the right program.
     */
    string output_string;
    /*
     FIRST, FOLLOW, PREDICT sets, contained in 3 maps.
     */
    map<string, set<string>> first_map;
    map<string, set<string>> follow_map;
    map<string, set<string>> predict_map;
    set<string> eps = {"SL", "ET", "TT", "FT"};
    /*
     interpretor variable store map.
     */
    map<string, int> val_map;
    /*
     general function for this assignment;
     */
    void program();
    treeNode* stmt_list(set<string> f_set);
    treeNode* stmt();
    treeNode* relat(set<string> f_set);
    treeNode* expr(set<string> f_set);
    treeNode* expr_tail(treeNode *r, set<string> f_set);
    treeNode* term(set<string> f_set);
    treeNode* term_tail(treeNode *e, set<string> f_set);
    treeNode* factor();
    treeNode* factor_tail(treeNode *t, set<string> f_set);
    string add_op();
    string mul_op();
    string ro_op();

    /*
     some helper functions for
         1. error printing,
         2. initial frist, follow, predict set.
         3. delete the tree allocated memory.
     */
    void init();
    void init_first();
    void init_follow();
    void init_predict();
    
    void error();
    string match(token expected);
    void deleteTree(treeNode* t);
    
    /*
     execute unit.
     to help execute the input program.
     */
    const int t_False = 0;
    const int t_True = 1;
    void run();
    int execute(treeNode* t);
    int exec_list(treeNode* t);
    bool contains(string str, const char* cstr);
    void exec_assign(treeNode* t);
    int eval_relat(treeNode* t);
    int eval_expr(treeNode* t);
    int cmp_val(string opr, int l_val, int r_val);
    int math_oper(char c, int l_val, int r_val);
    
};

#endif
