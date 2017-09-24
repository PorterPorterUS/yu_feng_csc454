#include "scan.h"
#include "parse.h"


void myParse::error () {
    std::cout << "syntax error" << std::endl;
    exit (1);
}

string myParse::match (token expected) {
    string token = "";
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        token = names[input_token];
        if (input_token == t_id || input_token == t_literal) {
            cout << ": " << myScan::get_token_image();
            token = "\"" + myScan::get_token_image() + "\"";
        }
        cout << endl;
        input_token = myScan::scan();
    }
    else error();
    return token;
}

void myParse::addIndent() {
    for (int i = 0; i < indentation; i++) {
        output_string += "  ";
    }
}

void myParse::addSpace() {
    output_string += " ";
}

void myParse::addReturn() {
    output_string += "\n";
    addIndent();
}

void printTree(treeNode *t) {
    if (t != nullptr) {
        cout << (*t).value;
        if ((*t).left != nullptr) {
            cout << " ";
            printTree((*t).left);
        }
        if ((*t).right != nullptr) {
            cout << " ";
            printTree((*t).right);
        }
        if (((*t).value.find("do") != string::npos) ||
            ((*t).value.find("if") != string::npos)) {
            cout << "\n ";
        }
        if ((*t).value.front() == '(') {
            cout << ")";
        }
        
        if ((*t).value.find('[') != string::npos) {
            cout << "\n ]";
        }
    }
}


void myParse::program () {
    indentation = 0;
    root = new treeNode("(program\n[", nullptr, nullptr);
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
        case t_if:
        case t_do:
        case t_check:
            cout << "predict program --> stmt_list eof" << endl;
            (*root).left = stmt_list();
            match (t_eof);
            break;
        default: error ();
    }
}

treeNode* myParse::stmt_list () {
    treeNode* sl = new treeNode("\n", nullptr, nullptr);
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            cout << "predict stmt_list --> stmt stmt_list" << endl;
            (*sl).left = stmt ();
            (*sl).right = stmt_list ();
            break;
        /* set<string> SL = {"$$", "od", "fi"}; */
        case t_eof:
        case t_od:
        case t_fi:
            cout << "predict stmt_list --> epsilon" << endl;
            delete sl;
            sl = nullptr;
            break;          /*  epsilon production */
        default: error ();
    }
    return sl;
}

treeNode* myParse::stmt () {
    treeNode* s = new treeNode("(", nullptr, nullptr);
    switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets expr" << endl;
            (*s).left = new treeNode(match(t_id), nullptr, nullptr);
            
            (*s).value += match(t_gets);
            (*s).right = expr();
            break;
        case t_read:
            cout << "predict stmt --> read id" << endl;
            (*s).value += match(t_read);
            (*s).value += " " + match(t_id);
            break;
        case t_write:
            cout << "predict stmt --> write relat" << endl;
            (*s).value += match(t_write);
            (*s).right = expr();
            break;
        case t_if:
            cout << "predict stmt --> if relat stmt_list fi" << endl;
            (*s).value += match (t_if) + "\n";
            (*s).left = relat();
            (*(*s).left).value.insert(0, "(");
            (*s).right = stmt_list();
            match(t_fi);
            break;
        case t_do:
            cout << "predict stmt --> do stmt_list od" << endl;
            (*s).value += match(t_do) + "\n  [";
            (*s).left = stmt_list();
            match(t_od);
            break;
        case t_check:
            cout << "predict stmt --> check relat" << endl;
            (*s).value += match(t_check);
            (*s).left = relat();
            break;
        default: error ();
    }
    return s;
}

treeNode* myParse::relat() {
    treeNode* r = new treeNode("", nullptr, nullptr);
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict relat --> expr expr_tail" << endl;
            (*r).left = expr();
            r = expr_tail(r);
            break;
        default: error ();
    }
    return r;
}

treeNode* myParse::expr () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail" << endl;
            return term_tail(term());
            break;
        default: error ();
    }
    return nullptr;
}

treeNode* myParse::expr_tail(treeNode *r) {
    switch (input_token) {
        case t_equal:
        case t_comp:
        case t_less:
        case t_greater:
        case t_lequal:
        case t_gequal:
            cout << "predict expr_tail --> ro_op expr" << endl;
            (*r).value = ro_op();
            (*r).right = expr();
            break;
/* FOLLOW set  --> set<string> ET = {")", "fi". "if", "$$",
             "id", "read", "write", "do", "od", "check"};  */
        case t_rparen:
        case t_fi:
        case t_if:
        case t_id:
        case t_read:
        case t_write:
        case t_do:
        case t_od:
        case t_check:
        case t_eof:
            cout << "predict expr_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: myParse::error ();
    }
    return r;
}

treeNode* myParse::term() {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail" << endl;
            return factor_tail(factor());
        default: myParse::error();
    }
    return nullptr;
}

treeNode* myParse::term_tail(treeNode *t) {
    /* predict_map["TT_AO"] = {"+", "-"}; */
    switch (input_token) {
        case t_add:
        case t_sub: {
            cout << "predict term_tail --> add_op term term_tail" << endl;
            treeNode* tt = new treeNode(("(" + add_op()), t, term());
            return term_tail(tt);
        }
/* FOLLOW set  --> set<string> TT = {")", "fi" , "if", "$$",
             "==", "<>", ">", "<", "<=", ">=",
             "id", "read", "write", "do", "od", "check"};*/
        case t_rparen:
        case t_fi:
        case t_if:
        case t_equal:
        case t_comp:
        case t_less:
        case t_greater:
        case t_lequal:
        case t_gequal:
        case t_id:
        case t_read:
        case t_write:
        case t_do:
        case t_od:
        case t_check:
        case t_eof:
            cout << "predict term_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: myParse::error ();
    }
    return t;
}

treeNode* myParse::factor () {
    switch (input_token) {
        case t_id: {
            cout << "predict factor --> id" << endl;
            return new treeNode( ("(id " + match(t_id)),
                                nullptr, nullptr);
        }
        case t_literal: {
            cout << "predict factor --> num" << endl;
            return new treeNode("(num " + match(t_literal), nullptr, nullptr);
        }
        case t_lparen: {
            cout << "predict factor --> ( expr )" << endl;
            treeNode* f;
            match (t_lparen);
            f = relat();
            (*f).value.insert(0, "(");
            match (t_rparen);
            return f;
        }
        default: error ();
    }
    return nullptr;
}

treeNode* myParse::factor_tail(treeNode* f) {
    switch (input_token) {
        case t_mul:
        case t_div: {
            cout <<"predict factor_tail --> mul_op factor factor_tail"
                << endl;
            treeNode* t = new treeNode(("("+mul_op()), f, factor());
            return factor_tail(t);
        }
/* FOLLOW set --> set<string> FT = {")", "fi", "$$", "+", "-",
             "==", "<>", ">", "<", "<=", ">=",
             "id", "read", "write", "do", "od", "check"};*/
        case t_rparen:
        case t_fi:
        case t_if:
        case t_add:
        case t_sub:
        case t_equal:
        case t_comp:
        case t_less:
        case t_greater:
        case t_lequal:
        case t_gequal:
        case t_id:
        case t_read:
        case t_write:
        case t_do:
        case t_od:
        case t_check:
        case t_eof:
            cout << "predict factor_tail --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error ();
    }
    return f;
}

string myParse::add_op () {
    switch (input_token) {
        case t_add: {
            cout << "predict add_op --> add" << endl;
            return match (t_add);
        }
        case t_sub: {
            cout << "predict add_op --> sub" << endl;
            return match (t_sub);
        }
        default: error ();
    }
    return nullptr;
}

string myParse::mul_op () {
    switch (input_token) {
        case t_mul: {
            cout << "predict mul_op --> mul" << endl;
            return match(t_mul);
        }
        case t_div: {
            cout << "predict mul_op --> div" << endl;
            return match(t_div);
        }
        default: error();
    }
    return nullptr;
}

string myParse::ro_op () {
    switch (input_token) {
        case t_equal: {
            cout << "predict ro_op --> ==" << endl;
            return match(t_equal);
        }
        case t_comp: {
            cout << "predict ro_op --> <>" << endl;
            return match(t_comp);
        }
        case t_greater: {
            cout << "predict ro_op --> >" << endl;
            return match(t_greater);
        }
        case t_less: {
            cout << "predict ro_op --> <" << endl;
            return match(t_less);
        }
        case t_lequal: {
            cout << "predict ro_op --> >=" << endl;
            return match(t_lequal);
        }
        case t_gequal: {
            cout << "predict ro_op --> <=" << endl;
            return match(t_gequal);
        }
        default: error();
    }
    return nullptr;
}

int myParse::main () {
    input_token = myScan::scan();
    program ();
    cout << "\n\n***************************************\n\n";
    printTree(root);
    return 0;
}


/* the following code is only to generate the FIRST and FOLLOW sets and PREDICT sets. */
void myParse::init() {
    init_first();
    init_follow();
    init_predict();
}

void myParse::init_first() {
    set<string> ro = {"==", "<>", ">", "<", "<=", ">="};
    first_map["ro"] = ro;
    set<string> ao = {"+", "-"};
    first_map["ao"] = ao;
    set<string> mo = {"*", "/"};
    first_map["mo"] = mo;
    
    set<string> P = {"$$", "id", "read", "write", "if", "do", "check"};
    first_map["P"] = P;
    set<string> S = {"id", "read", "write", "if", "do", "check"};
    first_map["S"] = S;
    set<string> SL = {"id", "read", "write", "if", "do", "check"};
    first_map["SL"] = SL;
    
    set<string> F = {"(", "id", "lit"};
    first_map["F"] = F;
    set<string> FT = {"*", "/"};
    first_map["FT"] = FT;
    
    set<string> T = {"(", "id", "lit"};
    first_map["T"] = T;
    set<string> TT = {"+", "-"};
    first_map["TT"] = TT;
    
    set<string> E = {"(", "id", "lit"};
    first_map["E"] = E;
    set<string> ET = {"==", "<>", ">", "<", "<=", ">="};
    first_map["ET"] = ET;
    
    set<string> R = {"(", "id", "lit"};
    first_map["R"] = R;
}
void myParse::init_follow() {
    set<string> ro = {"(", "id", "lit"};
    follow_map["ro"] = ro;
    set<string> ao = {"(", "id", "lit"};
    follow_map["ao"] = ao;
    set<string> mo = {"(", "id", "lit"};
    follow_map["mo"] = mo;
    
    set<string> P = {};
    follow_map["P"] = P;
    set<string> S = {"$$", "id", "read", "write", "if", "fi", "do", "check"};
    follow_map["S"] = S;
    set<string> SL = {"$$", "od", "fi"};
    follow_map["SL"] = SL;
    
    set<string> F = {")", "fi", "*", "/", "$$", "+", "-",
                    "==", "<>", ">", "<", "<=", ">=",
                "id", "read", "write", "do", "check"};
    follow_map["F"] = F;
    set<string> FT = {")", "fi" , "if", "$$", "+", "-",
                    "==", "<>", ">", "<", "<=", ">=",
                    "id", "read", "write", "do", "od", "check"};
    follow_map["FT"] = FT;
    
    set<string> T = {")", "fi", "if", "$$", "+", "-",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "od", "check"};
    follow_map["T"] = T;
    set<string> TT = {")", "fi", "fi", "$$",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "od", "check"};
    follow_map["TT"] = TT;
    
    set<string> E = {")", "fi", "if", "$$",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "od", "check"};
    follow_map["E"] = E;
    set<string> ET = {")", "fi", "if", "$$",
            "id", "read", "write", "do", "od", "check"};
    follow_map["ET"] = ET;
    
    set<string> R = {")", "fi", "if", "$$",
        "id", "read", "write", "do", "od", "check"};
    follow_map["R"] = R;
}

void myParse::init_predict() {
    set<string> P_SL (first_map["P"]);
    predict_map["P_SL"] = P_SL;
    
    set<string> SL_S_SL (first_map["S"]);
    predict_map["SL_S_SL"] = SL_S_SL;
    
    set<string> SL_EPS = {follow_map["SL"]};
    predict_map["SL_EPS"] = SL_EPS;

    predict_map["S_ID"] = {"id"};

    predict_map["S_READ"] = {"read"};
    
    predict_map["S_WRITE"] = {"write"};
    
    predict_map["S_IF"] = {"if"};
    
    predict_map["S_DO"] = {"do"};
    
    predict_map["S_CHECK"] = {"check"};
    
    set<string> R_E_ET (first_map["E"]);
    predict_map["R_E_ET"] = R_E_ET;
    
    set<string> E_T_TT (first_map["T"]);
    predict_map["E_T_TT"] = E_T_TT;
    
    set<string> T_F_FT (first_map["F"]);
    predict_map["T_F_FT"] = T_F_FT;
    
    predict_map["F_(R)"] = {"("};
    
    predict_map["F_ID"] = {"id"};
    
    predict_map["F_LIT"] = {"lit"};
    
    predict_map["ET_RO"] = {"==", "<>", ">", "<", "<=", ">="};
    
    set<string> ET_EPS (follow_map["ET"]);
    predict_map["ET_EPS"] = ET_EPS;
    
    predict_map["TT_AO"] = {"+", "-"};
    
    set<string> TT_EPS (follow_map["TT"]);
    predict_map["TT_EPS"] = TT_EPS;
    
    predict_map["FT_MO"] = {"*", "/"};
    
    set<string> FT_EPS (follow_map["FT"]);
    predict_map["FT_EPS"] = FT_EPS;
    
}
