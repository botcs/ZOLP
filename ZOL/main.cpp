#include <iostream>
#include <string>


struct AST //Abstract Syntax Tree
{
    struct node
    {
        AST::node* parent;
        enum{
            LITERAL,
            VARIABLE,
            OR,
            AND
        } type ;
        union{
            bool litVal; // for LITERAL evaluation
            char* varName; //for VARIABLE representation
            struct{
                AST::node * leftChild;
                AST::node * rightChild;
            };
        };
    };

    node * root;

    ~AST(){
        postDestruct(root);
    }

    void postDestruct(node* x){
        if (x->leftChild) postDestruct(x->leftChild);
        if (x->rightChild) postDestruct(x->rightChild);
        delete x;
    }
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
#include <vector>
struct RDparser
{
    AST::node * parseExpression();
    AST::node * parseTerm();
    AST::node * parseUnary();
    AST::node * parseFactor();
    AST::node * parseAtom();


};

int main()
{
    AST tree;

    return 0;
}
