#include "scan.h"

Token myScan::scan() {

    c = ' ';
    token_image = "";

    while (isspace(c)) {
        std::cin.get(c);
    }
    if (c == std::char_traits<char>::eof) {
        return Token::t_eof;
    }
    if (isalpha(c)) {
        do {
            token_image += c;
            std::cin.get(c);
        } while (isalpha(c) || isdigit(c) || c == '_');
        if (token_image.compare("read") == 0) {
            return Token::t_read;
        } else if (token_image.compare("write") == 0) {
            return Token::t_write;
        } else {
            return Token::t_id;
        }
    } else if (isdigit(c)) {
        do {
            token_image += c;
            std::cin.get(c);
        } while (isdigit(c));
        return Token::t_literal;
    } else {
        switch(c) {
            case ':': {
            if ((std::cin.get(c)) != '=') {
                    fprintf(stderr, "error\n");
                    exit(1);
                } else {
                    std::cin.get(c);
                    return t_gets;
                }
                break;
            }  
            case '+': 
            std::cin.get(c); return Token::t_add;
            case '-': 
            std::cin.get(c); return Token::t_sub;
            case '*': 
            std::cin.get(c); return Token::t_mul;
            case '/': 
            std::cin.get(c); return Token::t_div;
            case '(': 
            std::cin.get(c); return Token::t_lparen;
            case ')': 
            std::cin.get(c); return Token::t_rparen;
            default:
            std::cout << "ERROR::illegal token." << std::endl;
            exit(-1);
        }
    }
}