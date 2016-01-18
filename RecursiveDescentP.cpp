#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"

#define tk_ token::

token * RDparser::parseRightHalfExpr(token * leftChild){
    auto danglingNode = getNext();
    if( !(accept(tk_ OR) || accept(tk_ AND)) )
        throw ParseError("Dangling operator collision ...");


    danglingNode->left = leftChild;
    danglingNode->right = parseAnd();

    return danglingNode;
}

token * RDparser::parseAnd(){
    auto leftChild = parseOr();
    if (accept(tk_ OR)){
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
    if (accept(tk_ AND)){
        auto realAndNode = getAccepted();
        auto rightChild  = parseNot();
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseNot(){
    if(accept(tk_ NOT)){
        auto realNotNode = getAccepted();
        realNotNode->child = parseNot();
        return realNotNode;
    }
    return parseParen();
}
token * RDparser::parseParen(){

    if(accept(tk_ OPEN)){
        auto startingDepth = parenDepth;
        ++parenDepth;

        auto parenExpr = parseAnd();
        if(accept(tk_ CLOSE)) {
            --parenDepth;
            return parenExpr;
        }

        while(!complete() && parenDepth != startingDepth){
            parenExpr = parseRightHalfExpr(parenExpr);
            if(accept(tk_ CLOSE)) {
                --parenDepth;
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
    o << "\n\n";
}

std::vector<token*> RDparser::disassemblyToken (const std::string& T){

    std::vector<token*> trueTokens;
    size_t start = 0;
    while(start < T.size()){
        size_t minPos = T.size();
        std::string leftMostToken;
        for(auto p : token::TokenDict){
            auto pos = T.find(p.first, start);
            if( pos < minPos ){
                minPos = pos;
                leftMostToken = p.first;
                if(pos == start)
                    break;
            }
        }

        if(minPos < T.size()){ //INTERPRETABLE TOKEN FOUND
            if(minPos > start)
                trueTokens.push_back(new token(T.substr(start, minPos - start) ));

            trueTokens.push_back(new token(leftMostToken));
            start = minPos + leftMostToken.size();
        } else { //INTERPRETING WHOLE RIGHTMOST EXPRESSION AS VARIABLE
            trueTokens.push_back(new token(T.substr(start) ) );
            break;
        }



    }


    return trueTokens;
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
            std::vector<token*> trueTokens = disassemblyToken(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }
        else tokens.push_back(new token(tokenString) );

        o << "\"" << tokenString << "\"\t ";
        tokens.back()->print(o);
        o << " ;\n";
    }
    o << "Factorizing ended\n\n";

}

RDparser::RDparser(std::stringstream & Buffer){
    std::string tokenString;
    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            std::vector<token*> trueTokens = disassemblyToken(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }

        else tokens.push_back(new token(tokenString) );
    }


}

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
