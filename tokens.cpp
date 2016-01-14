#include "tokens.h"

std::unordered_map<std::string, token::T> token::TokenDict = {
    {"false", token::FALSE},
    {"FALSE", token::FALSE},
    {"F", token::FALSE},
    {"f", token::FALSE},

    {"true", token::TRUE},
    {"TRUE", token::TRUE},
    {"T", token::TRUE},
    {"t", token::TRUE},

    {"not", token::NOT},
    {"!", token::NOT},
    {"~", token::NOT},

    {"and", token::AND},
    {"&&", token::AND},
    {"&", token::AND},

    {"or", token::OR},
    {"||", token::OR},
    {"|", token::OR},

    {"(", token::OPEN},
    {"{", token::OPEN},
    {"[", token::OPEN},

    {"]", token::CLOSE},
    {"}", token::CLOSE},
    {")", token::CLOSE},
};

size_t token::maxTokenLength = 5;
