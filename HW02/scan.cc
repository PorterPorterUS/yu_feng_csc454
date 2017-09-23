#include "scan.h"

const string myScan::get_token_image() {
    return token_image;
}

token myScan::scan() {
    c = ' ';
    token_image.clear();

    while (isspace(c)) {
        cin.get(c);
    }
    if (c == char_traits<char>::eof()) {
        return t_eof;
    }
    if (c == '$') {
        token_image += c;
        cin.get(c);
        if (c == '$') {
            token_image += c;
            cout << "scanner get \"" << token_image << "\"" << endl;
            return t_eof;
        } else {
            cerr << "error" << endl;
            exit(1);
        }
    }
    
    if (isalpha(c)) {
        do {
            token_image += c;
            cin.get(c);
        } while (isalpha(c) || isdigit(c) || c == '_');
        cout << "scanner get \"" << token_image << "\"" << endl;
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
        } while (isdigit(c));
        return t_literal;
    } else {
        switch(c) {
            case ':': {
            cin.get(c);
            if (c != '=') {
                    cerr << "error" << endl;
                    exit(1);
                } else {
                    cin.get(c);
                    return t_gets;
                }
                break;
            }  
            case '+': 
                cin.get(c); return t_add;
            case '-': 
                cin.get(c); return t_sub;
            case '*': 
                cin.get(c); return t_mul;
            case '/': 
                cin.get(c); return t_div;
            case '(': 
                cin.get(c); return t_lparen;
            case ')': 
                cin.get(c); return t_rparen;
            case '=':
                cin.get(c);
                if (c != '=') {
                    cerr << "error" << endl;
                    exit(1);
                } else {
                    cin.get(c);
                    return t_equal;
                }
                break;
            case '<':
                cin.get(c);
                switch (c) {
                    case '>':
                        cin.get(c); return t_comp;
                    case '=':
                        cin.get(c); return t_lequal;
                    default:
                        if (isspace(c)) {
                            return t_less;
                        } else {
                            cerr << "error" << endl;
                            exit(1);
                        }
                        break;
                }
            case '>':
                cin.get(c);
                switch (c) {
                    case '=':
                        cin.get(c); return t_gequal;
                    default:
                        if (isspace(c)) {
                            return t_greater;
                        } else {
                            cerr << "error" << endl;
                            exit(1);
                        }
                        break;
                }
            default:
            cerr << "error" << endl;
            exit(-1);
        }
    }
}
