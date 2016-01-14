#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>



#include "AST.h"


std::unordered_map<std::string, token::T> token::UsableTokens = {
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

int main()
{

    using namespace std;
    try{
        AST tree;
        //rdp.tokens=vector<string>{"(", "first", "and", "second", ")",  "and", "third"};
        //rdp.tokens=vector<string>{"(", "first", "and", "second", ")",  "and", "not", "not", "true"};
        //rdp.tokens=vector<string>{"first", "and", "second", "and", "third"};
        stringstream ss("( [ false ]  ) and A or B or (C and ! D or ! ~ K)");
        tree.parse(ss);
        tree.print(cout);

    } catch (exception& e)
    {
        cerr << e.what();
    }

    return 0;
}
