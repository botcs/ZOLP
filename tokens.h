#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED
#include <unordered_map>
#include <exception>
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


    static std::unordered_map<std::string, T> UsableTokens;

    union{
        const char* varName; //for VARIABLE representation
        struct{
            token * left;
            token * right;
        };
    };
    void print(std::ostream& o){
        switch (type){
        case TRUE:
            o << "TRUE";
            break;

        case FALSE:
            o << "FALSE";
            break;

        case VARIABLE:
            o << '[' << varName << ']';
            break;

        case NOT:
            o << "not";
            break;

        case OR:
            o << "or";
            break;

        case AND:
            o << "and";
            break;

        case OPEN:
            o << "OPEN";
            break;

        case CLOSE:
            o << "CLOSE";
            break;



        default:
            throw std::logic_error("Unrecognized token Type");
        }
    }
    token(T _type): type(_type){}
    token(std::string& tokenString): type(UsableTokens[tokenString]){
        if(type == VARIABLE)
            varName = tokenString.c_str();

    }
};




#endif // TOKENS_H_INCLUDED
