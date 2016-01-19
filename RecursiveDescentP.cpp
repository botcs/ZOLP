#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"
#include "LexAnalyser.h"


token * RDparser::parseRightHalfExpr(token * leftChild){
    auto danglingNode = getNext();
    if( !(accept(token::OR) || accept(token::AND)) )
        throw ParseError("Dangling operator collision ...");


    danglingNode->left = leftChild;
    danglingNode->right = parseAnd();

    return danglingNode;
}

token * RDparser::parseAnd(){
    auto leftChild = parseOr();
    if (accept(token::OR)){
        auto realOrNode = getAccepted();
        auto rightChild  = parseOr();
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;

        return realOrNode;
    }
    return leftChild;
}
token * RDparser::parseOr(){
    auto leftChild = parseNot();
    if (accept(token::AND)){
        auto realAndNode = getAccepted();
        auto rightChild  = parseNot();
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseNot(){
    if(accept(token::NOT)){
        auto negatedChild = parseNot();
        negatedChild->negated = !negatedChild->negated;
        return negatedChild;
    }
    return parseParen();
}
token * RDparser::parseParen(){

    if(accept(token::OPEN)){
        auto startingDepth = parenDepth;
        ++parenDepth;

        auto parenExpr = parseAnd();
        if(accept(token::CLOSE)) {
            --parenDepth;
            return parenExpr;
        }

        while(!complete() && parenDepth != startingDepth){
            parenExpr = parseRightHalfExpr(parenExpr);
            if(accept(token::CLOSE)) {
                --parenDepth;
                return parenExpr;
            }
        }




        throw ParseError("Missing parentheses");
    }
    if(accept(token::TRUE)){
        return getAccepted();
    }

    if(accept(token::FALSE)){
        return getAccepted();
    }

    if(accept(token::VARIABLE))
        return getAccepted();

    throw ParseError("Could not resolve Atomic expression");
}

void RDparser::print(std::ostream& o){
    o << "BUFFERED tokens at " << this << " :\n";
    for(auto t : tokens){
        o << " \"";
        t->print(o);
        o << "\"";
    }
    o << "\n\n";
}




RDparser::RDparser(std::stringstream & Buffer, std::ostream& o){
    ///IMPORTANT! In the verbose version the aggregarated expressions
    ///(i.e. "(AorBorC)" are depicted as VARIABLES, but are disassembled into
    ///smaller exceptions if there exists any

    o << "Parser initialized at " << this << "\n"
      << "\"input\"\t interpreted as\n"
      << "********************************************\n";
    std::string tokenString;

    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            std::vector<token*> trueTokens = Tokenize(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }
        else tokens.push_back(new token(tokenString) );

        o << "\"" << tokenString << "\"\t ";
        tokens.back()->print(o);
        o << " ;\n";
        o << "\n";
    }
    o << "Factorizing ended\n\n";

}

RDparser::RDparser(std::stringstream & Buffer){
    std::string tokenString;
    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            std::vector<token*> trueTokens = Tokenize(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }

        else tokens.push_back(new token(tokenString) );
    }


}

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
