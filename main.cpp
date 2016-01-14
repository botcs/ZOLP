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
        stringstream ss("first and ( second and not not true ) or false");
        tree.parse(ss);
        tree.print(cout);

    } catch (exception& e)
    {
        cout << e.what();
    }

    return 0;
}
