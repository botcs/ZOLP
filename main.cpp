#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>



#include "AST.h"

int main()
{

    using namespace std;
    try{
        AST tree;
        //rdp.tokens=vector<string>{"(", "first", "and", "second", ")",  "and", "third"};
        //rdp.tokens=vector<string>{"(", "first", "and", "second", ")",  "and", "not", "not", "true"};
        //rdp.tokens=vector<string>{"first", "and", "second", "and", "third"};
        stringstream ss("(AorB) AND C & (D | (D)) && false");
        tree.parse(ss, cout);
        tree.print(cout);




    } catch (exception& e)
    {
        cerr << e.what();
    }

    return 0;
}
