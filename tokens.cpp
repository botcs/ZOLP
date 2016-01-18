#include "tokens.h"

std::unordered_map<std::string, token::T> token::TokenDict = {
    {"false", token::FALSE},
    {"FALSE", token::FALSE},


    {"true", token::TRUE},
    {"TRUE", token::TRUE},

    {"not", token::NOT},
    {"NOT", token::NOT},
    {"!", token::NOT},
    {"~", token::NOT},

    {"and", token::AND},
    {"AND", token::AND},
    {"&&", token::AND},
    {"&", token::AND},

    {"or", token::OR},
    {"OR", token::OR},
    {"||", token::OR},
    {"|", token::OR},

    {"(", token::OPEN},
    {"{", token::OPEN},
    {"[", token::OPEN},

    {"]", token::CLOSE},
    {"}", token::CLOSE},
    {")", token::CLOSE},
};

void token::print(std::ostream& o){
    if (negated)
        o << "!";
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
