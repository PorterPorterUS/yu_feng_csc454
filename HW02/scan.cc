#include "scan.h"


void myScan::ckeckCharAndError(bool msg) {
    static string prev_line;
    if (msg) {
        cerr << "syntax error:: "<< "at line: " << line << ", col: " << col;
        if (curr_line.length() == 0) {
            cerr << "; please check \"" << prev_line << "\" <- here." << endl;
        } else {
            cerr << "; please check \"" << curr_line << "\" <- here." << endl;
        }
        
    } else {
        if (c == '\n') {
            line += 1;
            col = 0;
            prev_line = curr_line;
            curr_line = "";
        } else {
            col += 1;
            curr_line += c;
        }
    }
}

const string myScan::get_token_image() {
    return token_image;
}

token myScan::scan() {
    c = ' ';
    token_image.clear();

    while (isspace(c)) {
        cin.get(c);
        ckeckCharAndError(false);
    }
    if (c == char_traits<char>::eof()) {
        return t_eof;
    }
    if (c == '$') {
        token_image += c;
        cin.get(c);
        ckeckCharAndError(false);
        if (c == '$') {
            token_image += c;
            return t_eof;
        } else {
            ckeckCharAndError(true);
            exit(1);
        }
    }
    
    if (isalpha(c)) {
        do {
            token_image += c;
            cin.get(c);
            ckeckCharAndError(false);
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (token_image.compare("read") == 0) {
            return t_read;
        } else if (token_image.compare("write") == 0) {
            return t_write;
        } else if (token_image.compare("do") == 0) {
            return t_do;
        } else if (token_image.compare("od") == 0) {
            return t_od;
        } else if (token_image.compare("if") == 0) {
            return t_if;
        } else if (token_image.compare("fi") == 0) {
            return t_fi;
        } else if (token_image.compare("check") == 0) {
            return t_check;
        } else {
            return t_id;
        }
    } else if (isdigit(c)) {
        do {
            token_image += c;
            cin.get(c);
            ckeckCharAndError(false);
        } while (isdigit(c));
        return t_literal;
    } else {
        switch(c) {
            case ':': {
                cin.get(c);
                ckeckCharAndError(false);
                if (c != '=') {
                    ckeckCharAndError(true);
                    exit (1);
                } else {
                    cin.get(c);
                    ckeckCharAndError(false);
                    return t_gets;
                }
                break;
            }  
            case '+': 
                cin.get(c); ckeckCharAndError(false); return t_add;
            case '-': 
                cin.get(c); ckeckCharAndError(false); return t_sub;
            case '*': 
                cin.get(c); ckeckCharAndError(false); return t_mul;
            case '/': 
                cin.get(c); ckeckCharAndError(false); return t_div;
            case '(': 
                cin.get(c); ckeckCharAndError(false); return t_lparen;
            case ')': 
                cin.get(c); ckeckCharAndError(false); return t_rparen;
            case '=':
                cin.get(c);
                if (c != '=') {
                    ckeckCharAndError(true);
                    exit(1);
                } else {
                    cin.get(c);
                    ckeckCharAndError(false);
                    return t_equal;
                }
                break;
            case '<':
                cin.get(c);
                switch (c) {
                    case '>':
                        cin.get(c); ckeckCharAndError(false); return t_comp;
                    case '=':
                        cin.get(c); ckeckCharAndError(false); return t_lequal;
                    default:
                        if (isspace(c)) {
                            ckeckCharAndError(false);
                            return t_less;
                        } else {
                            ckeckCharAndError(true);
                            exit(1);
                        }
                        break;
                }
            case '>':
                cin.get(c);
                ckeckCharAndError(false);
                switch (c) {
                    case '=':
                        cin.get(c); ckeckCharAndError(false); return t_gequal;
                    default:
                        if (isspace(c)) {
                            return t_greater;
                        } else {
                            ckeckCharAndError(true);
                            exit(1);
                        }
                        break;
                }
            default:
            ckeckCharAndError(true);
            exit(-1);
        }
    }
}
