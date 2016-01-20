#ifndef RECURSIVEDESCENTP_H_INCLUDED
#define RECURSIVEDESCENTP_H_INCLUDED
#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>
#include <exception>

#include "AST.h"
#include "tokens.h"

class ParseError : public std::exception {
    const char* _detail;
    const char* what() const throw(){
        return _detail;
    };
public:
    ParseError( const std::string& detail) : _detail(detail.c_str()){}
};


using node_p = std::shared_ptr<AST::node>;
struct RDparser
{
    node_p parseExpression(){
        auto expr = parseAnd();
        while(!complete())
            expr = parseRightHalfExpr(expr);

        return expr;
    }

    node_p parseRightHalfExpr(node_p);
    node_p parseAnd();
    node_p parseOr();
    node_p parseNot();
    node_p parseParen();
    node_p parseAtom();

    std::vector<std::shared_ptr<token> > tokens;
    size_t index = 0;
    size_t parenDepth = 0;

    shared_ptr<token> getNext(){
        if(index >= tokens.size())
            throw ParseError("Overindexing");
        return tokens[index];
    }

    node_p getAccepted(){
        if(index-1 >= tokens.size())
            throw ParseError("Overindexing");

        return make_shared<AST::node>(tokens[index-1]);
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
