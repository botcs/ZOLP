#include <iostream>


struct AST //Abstract Syntax Tree
{
    struct node
    {
        //AST::node* parent;
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
                AST::node * left;
                AST::node * right;
            };
        };
    };

    node *root;

    ~AST(){
        postDestruct(root);
    }

    void postDestruct(node *x){
        if (x->leftChild) postDestruct(x->left);
        if (x->rightChild) postDestruct(x->right);
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

#include <exception>
class ParseError : public std::exception {

    const char* what() const throw();

};

#include <vector>


struct RDparser
{
    AST::node * parseExpression();
    AST::node * parseTerm();
    AST::node * parseUnary();
    AST::node * parseFactor();
    AST::node * parseAtom();

    vector<char*> tokens;
    size_t index = 0;

    char* getNext(){return tokens[index];}

    bool accept(char* s){
        if(getNext() == s){
            index++;
            return true;
        }
        return false;
    }
};

RDparser::parseExpression(){
    auto leftChild = parseTerm();
    if (accept("or")){
        auto rightChild  = parseTerm();
        auto realOrNode = new AST::node;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
    }
}
RDparser::parseTerm()
RDparser::parseUnary()
RDparser::parseFactor()
RDparser::parseAtom()

int main()
{
    AST tree;

    return 0;
}
