#include <iostream>


struct AST //Abstract Syntax Tree
{
    struct token
    {
        //AST::token* parent;
        enum{
            LITERAL,
            VARIABLE,
            NOT,
            OR,
            AND
        } type ;
        union{
            bool litVal; // for LITERAL evaluation
            char* varName; //for VARIABLE representation
            struct{
                AST::token * left;
                AST::token * right;
            };
        };
    };

    token *root;

    ~AST(){
        postDestruct(root);
    }

    void postDestruct(token *x){
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
    AST::token * parseExpression();
    AST::token * parseTerm();
    AST::token * parseUnary();
    AST::token * parseFactor();
    AST::token * parseAtom();

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
        auto realOrNode = new AST::token;
        realOrNode->type = AST::token::OR;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
        return realOrNode;
    }
    return leftChild;
}
RDparser::parseTerm(){
    auto leftChild = parseUnary();
    if (accept("and")){
        auto rightChild  = parseUnary();
        auto realAndNode = new AST::token;
        realAndNode->type = AST::token::OR;
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
RDparser::parseUnary(){
    if(accept("not")){
        auto realNotNode = new AST::token;
        realNotNode->type = AST::token::NOT;
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
RDparser::parseFactor(){
    if(accept("(")){
        auto parenExpr = parseExpression();
        if(accept(")")) return parenExpr;
        else throw ParseError("ParseError: Parentheses");
    }
    if(accept("true")){

    }
}
RDparser::parseAtom()

int main()
{
    AST tree;

    return 0;
}
