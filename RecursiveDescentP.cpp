#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"

#define tk_ token::

token * RDparser::parseRightHalfExpr(token * leftChild){
    auto floatingNode = new token(tk_ OR);
    if(accept(tk_ OR)) floatingNode->type = tk_ OR;
    else if(accept(tk_ AND)) floatingNode->type = tk_ AND;
    else {
        throw ParseError("Dangling operator collision ...");
    }

    floatingNode->left = leftChild;
    floatingNode->right = parseExpression();

    return floatingNode;
}

token * RDparser::parseExpression(){
    auto leftChild = parseTerm();
    if (accept(tk_ OR)){
        auto rightChild  = parseTerm();
        auto realOrNode = new token(tk_ OR);
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
        return realOrNode;
    }
    return leftChild;
}
token * RDparser::parseTerm(){
    auto leftChild = parseUnary();
    if (accept(tk_ AND)){
        auto rightChild  = parseUnary();
        auto realAndNode = new token(tk_ AND);
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseUnary(){
    if(accept(tk_ NOT)){
        auto realNotNode = new token(tk_ NOT);
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
token * RDparser::parseFactor(){
    if(accept(tk_ OPEN)){
        auto parenExpr = parseExpression();
        if(accept(tk_ CLOSE)) return parenExpr;
        else throw ParseError("Missing paren");
    }
    if(accept(tk_ TRUE)){
        return new token(tk_ TRUE);
    }

    if(accept(tk_ FALSE)){
        return new token (tk_ FALSE);
    }

    auto variable = new token(tk_ VARIABLE);
    variable->varName = getNext().varName;
    index++;
    return variable;
}
RDparser::RDparser(std::stringstream & Buffer4Parse){
        std::string tokenString;
        while(Buffer4Parse >> tokenString){
            tokens.emplace_back(tokenString);
        }

    }

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
