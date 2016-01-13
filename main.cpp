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
            const char* varName; //for VARIABLE representation
            struct{
                AST::token * left;
                AST::token * right;
            };
        };
    };

    token *root = nullptr;

    ~AST(){
        if(root)
            postDestruct(root);
    }

    void postDestruct(token *x){

        if (x->left) postDestruct(x->left);
        if (x->right) postDestruct(x->right);
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

    const char* what() const throw(){
        return "Parser fail";
    };

};

#include <vector>


struct RDparser
{
    AST::token * parseExpression();
    AST::token * parseTerm();
    AST::token * parseUnary();
    AST::token * parseFactor();
    AST::token * parseAtom();

    std::vector<const char*> tokens;
    size_t index = 0;

    const char* getNext(){return tokens[index];}

    bool accept(const char* s){
        if(index >= tokens.size()) throw ParseError();
        if(getNext() == s){
            index++;
            return true;
        }
        return false;
    }
};

AST::token * RDparser::parseExpression(){
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
AST::token * RDparser::parseTerm(){
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
AST::token * RDparser::parseUnary(){
    if(accept("not")){
        auto realNotNode = new AST::token;
        realNotNode->type = AST::token::NOT;
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
AST::token * RDparser::parseFactor(){
    if(accept("(")){
        auto parenExpr = parseExpression();
        if(accept(")")) return parenExpr;
        else throw ParseError();
    }
    if(accept("true")){
        auto literal = new AST::token;
        literal->type = AST::token::LITERAL;
        literal->litVal = true;
        return literal;
    }
    if(accept("false")){
        auto literal = new AST::token;
        literal->type = AST::token::LITERAL;
        literal->litVal = false;
        return literal;
    }

    auto variable = new AST::token;
    variable->type = AST::token::VARIABLE;
    variable->varName = getNext();
    index++;
    return variable;
}

int main()
{
    AST tree;
    RDparser rdp;
    rdp.tokens=std::vector<const char*>{"first", "second"};
    std::cout << rdp.parseUnary()->varName << "\n" << rdp.parseFactor()->varName;
    return 0;
}
