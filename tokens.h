#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED
#include <unordered_map>
#include <exception>
#include <iostream>
struct token
{
    enum T{
        //OPERANDICES
        VARIABLE = 0, //COMES HANDY IF TOKENDICTIONARY CAN'T FIND ANY ASSIGNED TOKEN
        TRUE,
        FALSE,

        //OPERATORS
        NOT,
        OR,
        AND,

        //PARENTHESES
        OPEN,
        CLOSE
    } type ;


    static std::unordered_map<std::string, T> TokenDict;
    bool negated = false;

    //union{
        std::string varName; //for VARIABLE representation
        struct{
            token * left;
            token * right;
        };
    //};
    void print(std::ostream& o);

    bool isAtom(){return type == FALSE || type == TRUE || type == VARIABLE;}
    bool isUnary(){return type == NOT;}
    bool isBinary(){return type == AND  || type == OR;}

    token(T _type): type(_type){}
    token(const std::string& tokenString): type(TokenDict[tokenString]){
        if(type == VARIABLE)
            varName = tokenString;

    }
};




#endif // TOKENS_H_INCLUDED
