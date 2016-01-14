#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"

#define tk_ token::

token * RDparser::parseRightHalfExpr(token * leftChild){
    auto floatingNode = getNext();
    if( !(accept(tk_ OR) || accept(tk_ AND) ) )
        throw ParseError("Dangling operator collision ...");


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

    auto variable = getNext();
    index++;
    return variable;
}
RDparser::RDparser(std::stringstream & Buffer, std::ostream& o){
    o << "Parser initialized at " << this << "\n"
      << "\"input\"\t interpreted as\n"
      << "********************************************\n";
    std::string tokenString;
    while(Buffer >> tokenString){
        o << "\"" << tokenString << "\"\t ";
        tokens.push_back(new token(tokenString) );
        tokens.back()->print(o);
        o << " ;\n";
    }
    o << "Factorizing ended\n\n";

}

RDparser::RDparser(std::stringstream & Buffer){
    std::string tokenString;
    while(Buffer >> tokenString)
        tokens.push_back(new token(tokenString) );

}

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
