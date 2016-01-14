#ifndef RECURSIVEDESCENTP_H_INCLUDED
#define RECURSIVEDESCENTP_H_INCLUDED
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

#include "tokens.h"

class ParseError : public std::exception {
    const char* _detail;
    const char* what() const throw(){
        return _detail;
    };
public:
    ParseError( std::string detail) : _detail(detail.c_str()){}
};



//
//GRAMMAR:
//expression -> term or term
//expression -> term
//
//term -> unary and unary
//term -> unary
//
//unary -> not factor
//uanry -> factor
//
//factor -> ( expression )
//factor -> atom
//
//atom -> true
//atom -> false
//atom -> variable


struct RDparser
{
    token * parseRightHalfExpr(token*);

    token * parseExpression();
    token * parseTerm();
    token * parseUnary();
    token * parseFactor();
    token * parseAtom();

    std::vector<token> tokens;
    size_t index = 0;

    token& getNext(){
        if(index >= tokens.size())
            throw ParseError("Overindexing");
        return tokens[index];
    }

    bool complete(){return tokens.size() == index;}

    bool accept(token::T type){
        if (complete()) return false;
        if(getNext().type == type){
            index++;
            return true;
        }
        return false;
    }
    RDparser(std::stringstream & Buffer);
};


#endif // RECURSIVEDESCENTP_H_INCLUDED
