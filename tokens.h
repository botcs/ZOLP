#ifndef TOKENS_H_INCLUDED
#define TOKENS_H_INCLUDED
struct token
{
    //token* parent;
    enum{
        LITERAL,
        VARIABLE,
        NOT,
        OR,
        AND
    } type ;
    union{
        bool litVal; // for LITERAL evaluation
        const char* varName; //for VARIABLE representation
        struct{
            token * left;
            token * right;
        };
    };
    void print(std::ostream& o){
        switch (type){
        case LITERAL:
            if(litVal) o << " T ";
            else       o << " F ";
            break;

        case VARIABLE:
            o << '[' << varName << ']';
            break;

        case NOT:
            o << " !";
            break;

        case OR:
            o << "or";
            break;

        case AND:
            o << "and";
            break;
        }
    }
};

#endif // TOKENS_H_INCLUDED
