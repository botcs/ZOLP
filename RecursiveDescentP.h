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
    ParseError( const std::string& detail) : _detail(detail.c_str()){}
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

    token * parseAnd();
    token * parseOr();
    token * parseNot();
    token * parseParen();
    token * parseAtom();

    std::vector<token*> tokens;
    size_t index = 0;
    size_t parenDepth = 0;

    token * getNext(){
        if(index >= tokens.size())
            throw ParseError("Overindexing");
        return tokens[index];
    }

    token * getAccepted(){
        if(index-1 >= tokens.size())
            throw ParseError("Overindexing");
        return tokens[index-1];
    }

    bool complete(){return tokens.size() == index;}

    bool accept(token::T type){
        if (complete()) return false;
        if(getNext()->type == type){

            ++index;
            return true;
        }
        return false;
    }
    void print(std::ostream&);

    RDparser(std::stringstream & Buffer);
    RDparser(std::stringstream & Buffer, std::ostream&);
};


#endif // RECURSIVEDESCENTP_H_INCLUDED
