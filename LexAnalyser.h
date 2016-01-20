#ifndef LEXANALYSER_H_INCLUDED
#define LEXANALYSER_H_INCLUDED
#include "tokens.h"
std::vector<shared_ptr<token> > Tokenize(const std::string& T);


std::vector<shared_ptr<token> > Tokenize (const std::string& T){

    std::vector<shared_ptr<token> > trueTokens;
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
                trueTokens.push_back(make_shared<token>(T.substr(start, minPos - start) ));

            trueTokens.push_back(make_shared<token>(leftMostToken));
            start = minPos + leftMostToken.size();
        } else { //INTERPRETING WHOLE RIGHTMOST EXPRESSION AS VARIABLE
            trueTokens.push_back(make_shared<token>(T.substr(start) ) );
            break;
        }



    }



    return trueTokens;
}


#endif // LEXANALYSER_H_INCLUDED
