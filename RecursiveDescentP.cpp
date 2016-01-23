#ifndef RECURSIVEDESCENTP_CPP_INCLUDED
#define RECURSIVEDESCENTP_CPP_INCLUDED

#include "RecursiveDescentP.h"
#include "LexAnalyser.h"


node_p RDparser::parseRightHalfExpr(node_p leftChild){

    if( !(accept(token::OR) || accept(token::AND)) )
        throw ParseError("Dangling operator collision ...");

    auto danglingNode = getAccepted();
    danglingNode->left = leftChild;
    danglingNode->right = parseAnd(danglingNode);

    return danglingNode;
}

std::shared_ptr<AST::node> RDparser::parseAnd(node_p parent){
    auto leftChild = parseOr(parent);
    if (accept(token::OR)){
        auto realOrNode = getAccepted();
        auto rightChild  = parseOr(realOrNode);
        leftChild->parent = realOrNode;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;

        return realOrNode;
    }
    return leftChild;
}
std::shared_ptr<AST::node> RDparser::parseOr(node_p parent){
    auto leftChild = parseNot(parent);
    if (accept(token::AND)){
        auto realAndNode = getAccepted();
        auto rightChild  = parseNot(realAndNode);
        leftChild->parent = realAndNode;
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
std::shared_ptr<AST::node> RDparser::parseNot(node_p parent){
    if(accept(token::NOT)){
        auto negatedChild = parseNot(parent);
        negatedChild->data->negated ^= true;
        return negatedChild;
    }
    return parseParen(parent);
}
std::shared_ptr<AST::node> RDparser::parseParen(node_p parent){

    if(accept(token::OPEN)){
        auto startingDepth = parenDepth;
        ++parenDepth;

        auto parenExpr = parseAnd(parent);
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

    if(accept(token::TRUE) || accept(token::FALSE) || accept(token::VARIABLE))
        return getAccepted();

    //SHAN'T REACH
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




RDparser::RDparser(std::istream & Buffer, std::ostream& o){
    ///IMPORTANT! In the verbose version the aggregarated expressions
    ///(i.e. "(AorBorC)" are depicted as VARIABLES, but are disassembled into
    ///smaller exceptions if there exists any

    o << "Parser initialized at " << this << "\n"
      << "\"input\"\t interpreted as\n"
      << "********************************************\n";
    std::string tokenString;

    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            auto trueTokens = Tokenize(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }
        else tokens.emplace_back(make_shared<token>(tokenString));

        o << "\"" << tokenString << "\"\t ";
        tokens.back()->print(o);
        o << " ;\n";
        o << "\n";
    }
    o << "Factorizing ended\n\n";

}

RDparser::RDparser(std::istream & Buffer){
    std::string tokenString;
    while(Buffer >> tokenString){
        if(!token::TokenDict.count(tokenString)){
            auto trueTokens = Tokenize(tokenString);
            tokens.reserve(tokens.size() + trueTokens.size());
            tokens.insert(tokens.end(), trueTokens.begin(), trueTokens.end());
        }

        else tokens.emplace_back(make_shared<token>(tokenString));
    }


}

#endif // RECURSIVEDESCENTP_CPP_INCLUDED
