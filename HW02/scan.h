#ifndef scan_h
#define scan_h

#include <isstream>
#include <string>
#include <cctype>


enum class Token {t_read, t_write, t_id, t_literal, t_gets,
                t_add, t_sub, t_mul, t_div, t_lparen, t_rparen, t_eof};

class myScan
{
public:
    myScan();
    ~myScan();
    static Token scan();

private:
    static string token_image;
    static char c;
};


#endif