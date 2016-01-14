#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED
#include <unordered_map>
struct token
{
    enum T{
        //OPERANDICES
        TRUE,
        FALSE,
        VARIABLE,

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

        default:
            o << "(";
        }
    }
    token(T _type): type(_type){}
    token(std::string& tokenString): type(UsableTokens[tokenString]){}
};




#endif // TOKENS_H_INCLUDED
