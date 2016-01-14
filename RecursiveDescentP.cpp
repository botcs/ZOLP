#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"

token * RDparser::parseRightHalfExpr(token * leftChild){
    auto floatingNode = new token;
    if(accept("or")) floatingNode->type = token::OR;
    else if(accept("and")) floatingNode->type = token::AND;
    else throw ParseError("operator could not be resolved!");

    floatingNode->left = leftChild;
    floatingNode->right = parseExpression();

    return floatingNode;
}

token * RDparser::parseExpression(){
    auto leftChild = parseTerm();
    if (accept("or")){
        auto rightChild  = parseTerm();
        auto realOrNode = new token;
        realOrNode->type = token::OR;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
        return realOrNode;
    }
    return leftChild;
}
token * RDparser::parseTerm(){
    auto leftChild = parseUnary();
    if (accept("and")){
        auto rightChild  = parseUnary();
        auto realAndNode = new token;
        realAndNode->type = token::AND;
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseUnary(){
    if(accept("not")){
        auto realNotNode = new token;
        realNotNode->type = token::NOT;
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
token * RDparser::parseFactor(){
    if(accept("(")){
        auto parenExpr = parseExpression();
        if(accept(")")) return parenExpr;
        else throw ParseError("Missing paren");
    }
    if(accept("true")){
        auto literal = new token;
        literal->type = token::LITERAL;
        literal->litVal = true;
        return literal;
    }
    if(accept("false")){
        auto literal = new token;
        literal->type = token::LITERAL;
        literal->litVal = false;
        return literal;
    }

    auto variable = new token;
    variable->type = token::VARIABLE;
    variable->varName = getNext().c_str();
    index++;
    return variable;
}
RDparser::RDparser(std::stringstream & Buffer4Parse){
        std::string token;
        while(Buffer4Parse >> token){
            tokens.emplace_back(token);
        }
        #ifndef NDEBUG
            for(auto a : tokens){
                std::cout << " \"" << a << "\" ";
            }
            std::cout << "\nTOKENIZING FINISHED\n\n";
        #endif // NDEBUG
    }

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
