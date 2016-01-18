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
        stringstream ss("!( notnot A and (B or D) and C)");
        tree.parse(ss, cout);

        tree.print(cout);

        tree.atomizeNegation(tree.root);

        cout<<"\n\n AFTER atomizeNegation:\n";
        tree.print(cout);





    } catch (exception& e)
    {
        cerr << e.what();
    }

    return 0;
}
