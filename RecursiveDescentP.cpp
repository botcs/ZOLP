#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"

#define tk_ token::

token * RDparser::parseRightHalfExpr(token * leftChild){
    auto danglingNode = getNext();
    if( !(accept(tk_ OR) || accept(tk_ AND) ) )
        throw ParseError("Dangling operator collision ...");


    danglingNode->left = leftChild;
    danglingNode->right = parseExpression();

    return danglingNode;
}

token * RDparser::parseExpression(){
    auto leftChild = parseTerm();
    if (accept(tk_ OR)){
        auto realOrNode = getAccepted();
        auto rightChild  = parseTerm();
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;

        return realOrNode;
    }
    return leftChild;
}
token * RDparser::parseTerm(){
    auto leftChild = parseUnary();
    if (accept(tk_ AND)){
        auto realAndNode = getAccepted();
        auto rightChild  = parseUnary();
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseUnary(){
    if(accept(tk_ NOT)){
        auto realNotNode = getAccepted();
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
token * RDparser::parseFactor(){

    if(accept(tk_ OPEN)){
        auto startingDepth = parenDepth;
        ++parenDepth;

        auto parenExpr = parseExpression();
        if(accept(tk_ CLOSE)) {
            return parenExpr;
        }

        while(!complete() && parenDepth != startingDepth){
            parenExpr = parseRightHalfExpr(parenExpr);
            if(accept(tk_ CLOSE)) {
                return parenExpr;
            }
        }




        throw ParseError("Missing parentheses");
    }
    if(accept(tk_ TRUE)){
        return getAccepted();
    }

    if(accept(tk_ FALSE)){
        return getAccepted();
    }

    if(accept(tk_ VARIABLE))
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
    o << "\n";
}

std::vector<token*> disassemblyTokens (std::string& T){
    std::vector<token*> trueTokens;
//    size_t it = 0;
//    size_t frame = token::maxTokenLength;
//    while(it + frame < T.size()){
//        while(frame)
//        {
//            if(token::TokenDict[T.substr(it, frame)]);
//        }
//    }

    for(auto p : token::TokenDict){
        auto pos = T.find(p.first);
        if(pos != T.size()){
            trueTokens.push_back(new token( T.substr(0, pos - 1) ));
            trueTokens.push_back(new token(p.first));
            T = substr(pos + p.first.size());
        }
    }

}


RDparser::RDparser(std::stringstream & Buffer, std::ostream& o){
    o << "Parser initialized at " << this << "\n"
      << "\"input\"\t interpreted as\n"
      << "********************************************\n";
    std::string tokenString;

    while(Buffer >> tokenString){
        o << "\"" << tokenString << "\"\t ";
        if(!token::TokenDict.count(tokenString)){

        }
        tokens.push_back(new token(tokenString) );
        tokens.back()->print(o);
        o << " ;\n";
    }
    o << "Factorizing ended\n\n";

}

RDparser::RDparser(std::stringstream & Buffer){
    std::string tokenString;
    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            auto trueTokens = disassemblyToken(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }

        tokens.push_back(new token(tokenString) );
    }


}

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
