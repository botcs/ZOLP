#ifndef AST_H_INCLUDED
#define AST_H_INCLUDED
#include <memory>
#include "tokens.h"
using namespace std;
struct AST //Abstract Syntax Tree
{
    struct node{
        shared_ptr<token> data;
        shared_ptr<node> left = nullptr;
        shared_ptr<node> right = nullptr;

        node(shared_ptr<token> embed):data(embed){};
        node(token::T _type):data(make_shared<token>(_type)){};
    };
    shared_ptr<node> root = nullptr;

    void print(ostream& o){
        printRaw(o, root);
    }

    void printRaw(ostream& o, shared_ptr<node> p, const string& prefix = "", bool isTail = true);
    void parse(stringstream& ss);
    void parse(stringstream& ss, ostream& o);

    void atomizeNegation(shared_ptr<node> x);
    void atomizeNegation(){atomizeNegation(root);}

    void CNF(shared_ptr<node> x);
    void CNF(){CNF(root);}

    ~AST(){postDestruct(root);}

    void postDestruct(shared_ptr<node> x);
};

#endif // AST_H_INCLUDED
