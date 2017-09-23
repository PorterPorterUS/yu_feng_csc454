#include "scan.h"
#include "parse.h"


void myParse::error () {
    std::cout << "syntax error" << std::endl;
    exit (1);
}

void myParse::match (token expected) {
    if (input_token == expected) {
        cout << "matched " << names[input_token];
        if (input_token == t_id || input_token == t_literal) {
            cout << ": " << myScan::get_token_image();
        }
        cout << endl;
        input_token = myScan::scan();
    }
    else error();
}



void myParse::program () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_eof:
        case t_if:
        case t_do:
        case t_check:
            cout << "predict program --> stmt_list eof" << endl;
            stmt_list();
            match (t_eof);
            break;
        default: error ();
    }
}

void myParse::stmt_list () {
    switch (input_token) {
        case t_id:
        case t_read:
        case t_write:
        case t_if:
        case t_do:
        case t_check:
            cout << "predict stmt_list --> stmt stmt_list" << endl;
            stmt ();
            stmt_list ();
            break;
        /* set<string> SL = {"$$", "od", "fi"}; */
        case t_eof:
        case t_od:
        case t_fi:
            cout << "predict stmt_list --> epsilon" << endl;
            break;          /*  epsilon production */
        default: error ();
    }
}

void myParse::stmt () {
    switch (input_token) {
        case t_id:
            cout << "predict stmt --> id gets expr" << endl;
            match (t_id);
            match (t_gets);
            expr ();
            break;
        case t_read:
            cout << "predict stmt --> read id" << endl;
            match (t_read);
            match (t_id);
            break;
        case t_write:
            cout << "predict stmt --> write relat" << endl;
            match(t_write);
            expr();
            break;
        case t_if:
            cout << "predict stmt --> if relat stmt_list fi" << endl;
             match (t_if);
            relat();
            stmt_list();
            match(t_fi);
            break;
        case t_do:
            cout << "predict stmt --> do stmt_list od" << endl;
            match(t_do);
            stmt_list();
            match(t_od);
            break;
        case t_check:
            cout << "predict stmt --> check relat" << endl;
            match(t_check);
            relat();
            break;
        default: error ();
    }
}

void myParse::relat() {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict relat --> expr expr_tail" << endl;
            expr ();
            expr_tail ();
            break;
        default: error ();
    }
}

void myParse::expr () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict expr --> term term_tail" << endl;
            term ();
            term_tail ();
            break;
        default: error ();
    }
}

void myParse::expr_tail () {
    switch (input_token) {
        case t_equal:
        case t_comp:
        case t_less:
        case t_greater:
        case t_lequal:
        case t_gequal:
            cout << "predict expr_tail --> ro_op expr" << endl;
            ro_op();
            expr();
            break;
            
/* FOLLOW set  --> set<string> ET = {")", "fi", "$$",
             "id", "read", "write", "do", "check"};  */
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
}

void myParse::term () {
    switch (input_token) {
        case t_id:
        case t_literal:
        case t_lparen:
            cout << "predict term --> factor factor_tail" << endl;
            factor ();
            factor_tail ();
            break;
        default: myParse::error ();
    }
}

void myParse::term_tail () {
    /* predict_map["TT_AO"] = {"+", "-"}; */
    switch (input_token) {
        case t_add:
        case t_sub:
            cout << "predict term_tail --> add_op term term_tail" << endl;
            add_op();
            term();
            term_tail();
            break;
            
/* FOLLOW set  --> set<string> TT = {")", "fi", "$$",
             "==", "<>", ">", "<", "<=", ">=",
             "id", "read", "write", "do", "check"};*/
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
}

void myParse::factor () {
    switch (input_token) {
        case t_id:
            cout << "predict factor --> id" << endl;
            match (t_id);
            break;
        case t_literal:
            cout << "predict factor --> literal" << endl;
            match (t_literal);
            break;
        case t_lparen:
            cout << "predict factor --> lparen expr rparen" << endl;
            match (t_lparen);
            relat();
            match (t_rparen);
            break;
        default: error ();
    }
}

void myParse::factor_tail () {
    switch (input_token) {
        case t_mul:
        case t_div:
            cout <<"predict factor_tail --> mul_op factor factor_tail"
                << endl;
            mul_op ();
            factor ();
            factor_tail ();
            break;
/* FOLLOW set --> set<string> FT = {")", "fi", "$$", "+", "-",
             "==", "<>", ">", "<", "<=", ">=",
             "id", "read", "write", "do", "check"};*/
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
}

void myParse::add_op () {
    switch (input_token) {
        case t_add:
            cout << "predict add_op --> add" << endl;
            match (t_add);
            break;
        case t_sub:
            cout << "predict add_op --> sub" << endl;
            match (t_sub);
            break;
        default: error ();
    }
}

void myParse::mul_op () {
    switch (input_token) {
        case t_mul:
            cout << "predict mul_op --> mul" << endl;
            match (t_mul);
            break;
        case t_div:
            cout << "predict mul_op --> div" << endl;
            match (t_div);
            break;
        default: error ();
    }
}

void myParse::ro_op () {
    switch (input_token) {
        case t_equal:
            cout << "predict mul_op --> mul" << endl;
            match (t_equal);
            break;
        case t_comp:
            cout << "predict mul_op --> div" << endl;
            match (t_comp);
            break;
        case t_greater:
            cout << "predict mul_op --> mul" << endl;
            match (t_greater);
            break;
        case t_less:
            cout << "predict mul_op --> div" << endl;
            match (t_less);
            break;
        case t_lequal:
            cout << "predict mul_op --> mul" << endl;
            match (t_lequal);
            break;
        case t_gequal:
            cout << "predict mul_op --> div" << endl;
            match (t_gequal);
            break;
        default: error ();
    }
}

int myParse::main () {
    // init();
    input_token = myScan::scan();
    program ();
    return 0;
}

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
    set<string> FT = {")", "fi", "$$", "+", "-",
                    "==", "<>", ">", "<", "<=", ">=",
                    "id", "read", "write", "do", "check"};
    follow_map["FT"] = FT;
    
    set<string> T = {")", "fi", "$$", "+", "-",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "check"};
    follow_map["T"] = T;
    set<string> TT = {")", "fi", "$$",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "check"};
    follow_map["TT"] = TT;
    
    set<string> E = {")", "fi", "$$",
            "==", "<>", ">", "<", "<=", ">=",
            "id", "read", "write", "do", "check"};
    follow_map["E"] = E;
    set<string> ET = {")", "fi", "$$",
            "id", "read", "write", "do", "check"};
    follow_map["ET"] = ET;
    
    set<string> R = {")", "fi", "$$",
        "id", "read", "write", "do", "check"};
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
