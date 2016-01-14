#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED

#include "RecursiveDescentP.h"

struct AST //Abstract Syntax Tree
{
    token *root = nullptr;

    void print(std::ostream& o){
        print(o, root);
    }

    void print(std::ostream& o, token* p, int indent = 0);

    void parse(std::stringstream& ss);
    void parse(std::stringstream& ss, std::ostream& o);

    ~AST(){postDestruct(root);}

    void postDestruct(token *x);
};

#endif // AST_H_INCLUDED
