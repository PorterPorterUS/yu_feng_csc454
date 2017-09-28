#include "parse.h"
/*
 constructor for myParser.
 */
myParse::myParse(){
    scanner = new myScan();
    root = new treeNode("(program\n[", nullptr, nullptr);
};
/*
 destructor for myParser.
 */
myParse::~myParse(){
    deleteTree(root);
};
/*
 helper function to delete the allocated memory.
 */
void myParse::deleteTree(treeNode* t) {
    if (t == nullptr) {
        return;
    } else {
        deleteTree((*t).left);
        deleteTree((*t).right);
        delete t;
    }
}

/*
 the error method is a wrapper function to call myScan::Error function.
 */
void myParse::error () {
    (*scanner).ckeckCharAndError(true);
    /*
     when isError set is true,
     the program will not print the syntax tree.
     */
    isError = true;
}

/*
 the same method to match the input_token with expected one.
 */
string myParse::match (token expected) {
    string token = "";
    if (input_token == expected) {
        if (ifDebug) cout << "matched " << names[input_token];
        token = names[input_token];
        if (input_token == t_id || input_token == t_literal) {
            if (ifDebug) cout << ": " << (*scanner).get_token_image();
            token = "\"" + (*scanner).get_token_image() + "\"";
        }
        if (ifDebug) cout << endl;
        if (input_token == t_eof) {
            return "$$";
        }
        input_token = (*scanner).scan();
        return token;
    }
    else {
        error();
        throw "throw the match error.";
    }
}

/*
 print the pre-build syntax tree,
 if the parsing has no error.
 it will print the node first, and then,
 print its children.
 */
void myParse::printTree(treeNode *t) {
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

/*
 Launch the program and start to parse.
 */
void myParse::program() {
    /* read token and if the tokens is FIRST(P), preceed to SL. */
    try {
        switch (input_token) {
            case t_id:
            case t_read:
            case t_write:
            case t_eof:
            case t_if:
            case t_do:
            case t_check:
                if (ifDebug) cout << "predict program --> stmt_list eof" << endl;
                (*root).left = stmt_list(follow_map["SL"]);
                match (t_eof);
                break;
            default: error ();
        }
    } catch (const char * err_msg) {
        /* catch the error if too many errors at the start of the program, it will exit. */
        cerr << "catch error at program: " << err_msg << endl;
        while (follow_map["P"].find(names[input_token]) == follow_map["P"].end()
                 && first_map["P"].find(names[input_token]) == first_map["P"].end()) {
            input_token = (*scanner).scan();
            if (ifDebug) cout << "token: " << names[input_token] << endl;
        };
        /*
             catch the error and re-run the program again.
         */
        if (first_map["P"].find(names[input_token]) != first_map["P"].end()) {
            program();
        }
    }
    
}
/*
 the state of stmt_list.
 */
treeNode* myParse::stmt_list(set<string> f_set) {
    treeNode* sl = new treeNode("\n", nullptr, nullptr);
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            if (ifDebug) cout << "predict stmt_list --> stmt stmt_list" << endl;
            (*sl).left = stmt();
            (*sl).right = stmt_list(f_set);
            break;
        /* FOLLOW set<string> SL = {"$$", "od", "fi"}; */
        case t_eof:
        case t_od:
        case t_fi:
            if (ifDebug) cout << "predict stmt_list --> epsilon" << endl;
            delete sl;
            sl = nullptr;
            if (f_set.find(names[input_token]) == f_set.end()) {
                error ();
                throw "error from stmt_list!!";
                /* to check where the error from. */
            }
            break;          /*  epsilon production */
        default: {
            error ();
            throw "error from stmt_list!!";
        }
    }
    return sl;
}

treeNode* myParse::stmt() {
    treeNode* s = new treeNode("(", nullptr, nullptr);
    try {
        switch (input_token) {
            case t_id:
                if (ifDebug) cout << "predict stmt --> id gets expr" << endl;
                (*s).left = new treeNode(match(t_id), nullptr, nullptr);
                
                (*s).value += match(t_gets);
                (*s).right = relat(follow_map["R"]);
                break;
            case t_read:
                if (ifDebug) cout << "predict stmt --> read id" << endl;
                (*s).value += match(t_read);
                (*s).value += " " + match(t_id);
                break;
            case t_write:
                if (ifDebug) cout << "predict stmt --> write relat" << endl;
                (*s).value += match(t_write);
                (*s).right = expr(follow_map["E"]);
                break;
            case t_if:
                if (ifDebug) cout << "predict stmt --> if relat stmt_list fi" << endl;
                (*s).value += match (t_if) + "\n";
                (*s).left = relat(follow_map["R"]);
                (*(*s).left).value.insert(0, "(");
                /*
                 specify the stmt_list only can be returned by "fi".
                 */
                (*s).right = stmt_list({"fi"});
                match(t_fi);
                break;
            case t_do:
                if (ifDebug) cout << "predict stmt --> do stmt_list od" << endl;
                (*s).value += match(t_do) + "\n  [";
                /*
                 specify the stmt_list only can be returned by "od".
                 */
                (*s).left = stmt_list({"od"});
                match(t_od);
                break;
            case t_check:
                if (ifDebug) cout << "predict stmt --> check relat" << endl;
                (*s).value += match(t_check);
                (*s).left = relat(follow_map["R"]);
                break;
            default: error (); throw "error from stmt!!";
        }
    } catch (const char *err_msg) {
        /* the error only handle, to search if the following token is in the first or follow set. */
        cerr << "catch error at statement: " << err_msg << endl;
        while (follow_map["S"].find(names[input_token]) == follow_map["S"].end()
               && first_map["S"].find(names[input_token]) == first_map["S"].end()) {
            input_token = (*scanner).scan();
        };
        /* if the token is in the frist set, start the new statement.
           Otherwise return to the next state.
         */
        if (first_map["S"].find(names[input_token]) != first_map["S"].end()) {
            return stmt();
        }
    }
    
    return s;
}
/*
 ralation expression parsing.
 */
treeNode* myParse::relat(set<string> f_set) {
    treeNode* r = new treeNode("", nullptr, nullptr);
    set<string> tmp (f_set);
    string cmp[] = {"==", "<>", ">", "<", "<=", ">="};
    /*
     based on the f_set, build a context-specific f_set for expr();
     */
    tmp.insert(cmp, cmp + 6);
    try {
        switch (input_token) {
            case t_id:
            case t_literal:
                if (ifDebug) cout << "predict relat --> expr expr_tail" << endl;
                /*
                 send a follow set here is aimed to give to FT and TT.
                 */
                (*r).left = expr(tmp);
                /*
                 use try catch to catch content-specific error.
                 */
                try {
                    /*
                     here, only pass the ET follow set.
                     */
                    r = expr_tail(r, f_set);
                } catch (const char * err_msg) {
                    cerr << "catch error at relation: " << err_msg << endl;
                    while (f_set.find(names[input_token]) == f_set.end()
                           && first_map["ET"].find(names[input_token]) == first_map["ET"].end()) {
                        input_token = (*scanner).scan();
                    };
                    /*
                     if the follow token is in FIRST("ET"), try ET again.
                     otherwise, the token is in f_set. return directly.
                     */
                    if (first_map["ET"].find(names[input_token]) !=
                        first_map["ET"].end()) {
                        r = expr_tail(r, f_set);
                    }
                }
                break;
            /*
             here is the context-specific error detect for ")".
             */
            case t_lparen:
                if (ifDebug) cout << "predict relat --> ( expr expr_tail )" << endl;
                (*r).left = expr(tmp);
                /*
                 in the try-catch, only pass the {")"} in the follow set to TT and FT.
                 */
                try {
                    r = expr_tail(r, {")"});
                } catch (const char * err_msg) {
                    cerr << "catch error at relation: " << err_msg << endl;
                    while (first_map["ET"].find(names[input_token]) == first_map["ET"].end()
                           && input_token != t_rparen) {
                        input_token = (*scanner).scan();
                    };
                    /*
                     if the next token is the first of expr_tail, try it again.
                     otherwise, it is ")", return to the next state.
                     */
                    if (first_map["ET"].find(names[input_token]) !=
                        first_map["ET"].end()) {
                        r = expr_tail(r, {")"});
                    }
                }
                break;
            default: error (); throw "error from the relat !!";
        }
    } catch (const char * err_msg) {
        /*
         the error from relation and try to the relation parsing agian.
         Caution:: the follow set is using the initial given f_set!!
         */
        cerr << "catch error at relat from " << err_msg << endl;
        
        while (f_set.find(names[input_token]) == f_set.end()
               && first_map["R"].find(names[input_token]) == first_map["R"].end()) {
            input_token = (*scanner).scan();
        };
        if (first_map["R"].find(names[input_token]) !=
            first_map["R"].end()) {
            return relat(f_set);
        }
    }
    return r;
}

/*
 parsing expersion. the parameter is used only for passing to the FT and TT.
 */
treeNode* myParse::expr(set<string> f_set) {
    static token prev_token = input_token;
    try {
        switch (input_token) {
            case t_id:
            case t_literal:
                if (ifDebug) cout << "predict expr --> term term_tail" << endl;
                return term_tail(term(follow_map["T"]), f_set);
                break;
            case t_lparen:
                if (ifDebug) cout << "predict expr --> (term term_tail)" << endl;
                return term_tail(term(follow_map["T"]), {")"});
                break;
            default: error (); throw "error from expr !!";
        }
    } catch (const char *err_msg) {
        if (ifDebug) cout << "catch error at expr: " << err_msg << endl;
        /*
         here using the follow set to check if the next token is in the follow set.
         otherwise start the expression parsing again.
         */
        if (prev_token == t_lparen) {
            while (input_token != t_rparen &&
                   first_map["E"].find(names[input_token]) == first_map["E"].end()) {
                input_token = (*scanner).scan();
            };
        } else {
            while (f_set.find(names[input_token]) == f_set.end() &&
                   first_map["E"].find(names[input_token]) == first_map["E"].end()) {
                input_token = (*scanner).scan();
            };
        }
        
        /* parsing the expr again. */
        if (first_map["E"].find(names[input_token]) !=
            first_map["E"].end()) {
            return expr(f_set);
        }
    }
    /* return nullptr is only to stop compiler complain. */
    return nullptr;
}

/*
 the treenode r is the leading relation node
 and f_set is the context-specific follow set.
 */
treeNode* myParse::expr_tail(treeNode *r, set<string> f_set) {
    switch (input_token) {
        case t_equal:
        case t_comp:
        case t_less:
        case t_greater:
        case t_lequal:
        case t_gequal:
            if (ifDebug) cout << "predict expr_tail --> ro_op expr" << endl;
            (*r).value = ro_op();
            (*r).right = expr(f_set);
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
            /*
             throw exception if the token is not in "context-specific" follow set.
             */
            if (ifDebug) cout << "predict expr_tail --> epsilon" << endl;
            if (f_set.find(names[input_token]) == f_set.end()) {
                error ();
                throw "error from expr_tail!!";
            }
            break;          /*  epsilon production */
        default: error (); throw "error from expr_tail!!";
    }
    return r;
}

treeNode* myParse::term(set<string> f_set) {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            if (ifDebug) cout << "predict term --> factor factor_tail" << endl;
            /*
             this is call factor() first,
             and then, let the factor_tail pointer
             point to previous factor.
             */
            return factor_tail(factor(), f_set);
        default: error(); throw "error from term()!!";
    }
    return nullptr;
}

treeNode* myParse::term_tail(treeNode *t, set<string> f_set) {
    /* predict_map["TT_AO"] = {"+", "-"}; */
    switch (input_token) {
        case t_add:
        case t_sub: {
            if (ifDebug) cout << "predict term_tail --> add_op term term_tail" << endl;
            treeNode* tt = new treeNode(("(" + add_op()), t, term(f_set));
            return term_tail(tt, f_set);
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
            /* throw context specific exception. */
            if (ifDebug) cout << "predict term_tail --> epsilon" << endl;
            if (f_set.find(names[input_token]) == f_set.end()) {
                error ();
                throw "error from term_tail!!";
            }
            break;          /*  epsilon production */
        default: error (); throw "error from term_tail!!";
    }
    return t;
}

treeNode* myParse::factor () {
    switch (input_token) {
        case t_id: {
            if (ifDebug) cout << "predict factor --> id" << endl;
            return new treeNode( ("(id " + match(t_id)),
                                nullptr, nullptr);
        }
        case t_literal: {
            if (ifDebug) cout << "predict factor --> num" << endl;
            return new treeNode("(num " + match(t_literal), nullptr, nullptr);
        }
        case t_lparen: {
            if (ifDebug) cout << "predict factor --> ( expr )" << endl;
            treeNode* f;
            match (t_lparen);
            f = relat({")"});
            (*f).value.insert(0, "(");
            match (t_rparen);
            return f;
        }
        default: error (); throw "error from factor!!";
    }
    return nullptr;
}

treeNode* myParse::factor_tail(treeNode* f, set<string> f_set) {
    switch (input_token) {
        case t_mul:
        case t_div: {
            if (ifDebug) cout <<"predict factor_tail --> mul_op factor factor_tail"
                << endl;
            treeNode* t = new treeNode(("("+mul_op()), f, factor());
            return factor_tail(t, f_set);
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
            /* throw context specific exception. */
            if (ifDebug) cout << "predict factor_tail --> epsilon" << endl;
            if (f_set.find(names[input_token]) == f_set.end()) {
                error ();
                throw "error from factor_tail!!";
            }
            break;          /*  epsilon production */
        default: error (); throw "error from factor_tail!!";
    }
    return f;
}

string myParse::add_op () {
    switch (input_token) {
        case t_add: {
            if (ifDebug) cout << "predict add_op --> add" << endl;
            return match (t_add);
        }
        case t_sub: {
            if (ifDebug) cout << "predict add_op --> sub" << endl;
            return match (t_sub);
        }
        default: error (); throw "error from add_op() !!";
    }
    return nullptr;
}

string myParse::mul_op () {
    switch (input_token) {
        case t_mul: {
            if (ifDebug) cout << "predict mul_op --> mul" << endl;
            return match(t_mul);
        }
        case t_div: {
            if (ifDebug) cout << "predict mul_op --> div" << endl;
            return match(t_div);
        }
        default: error(); throw "error from mul_op() !!";
    }
    return nullptr;
}

string myParse::ro_op () {
    switch (input_token) {
        case t_equal: {
            if (ifDebug) cout << "predict ro_op --> ==" << endl;
            return match(t_equal);
        }
        case t_comp: {
            if (ifDebug) cout << "predict ro_op --> <>" << endl;
            return match(t_comp);
        }
        case t_greater: {
            if (ifDebug) cout << "predict ro_op --> >" << endl;
            return match(t_greater);
        }
        case t_less: {
            if (ifDebug) cout << "predict ro_op --> <" << endl;
            return match(t_less);
        }
        case t_lequal: {
            if (ifDebug) cout << "predict ro_op --> >=" << endl;
            return match(t_lequal);
        }
        case t_gequal: {
            if (ifDebug) cout << "predict ro_op --> <=" << endl;
            return match(t_gequal);
        }
        default: error(); throw "error from mul_op() !!";
    }
    return nullptr;
}

int myParse::main (bool debug) {
    isError = false;
    init();
    ifDebug = debug;
    input_token = (*scanner).scan();
    program ();
    
    if (!isError) {
        cout << "\n\n********** PROGRAM OUTPUT **************\n\n";
        printTree(root);
    }
    cout << "\n\n********** OUTPUT END **************\n\n";
    return 0;
}


/*
 the following code is only to generate the FIRST and FOLLOW sets and PREDICT sets.
 */
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
