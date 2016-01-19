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
        stringstream ss("a or b or c or a or b or c");
        tree.parse(ss, cout);

        cout<<"\n\n INITIAL Parsing:\n";
        tree.print(cout);

        cout<<"\n\n AFTER atomizeNegation:\n";
        tree.atomizeNegation(tree.root);
        tree.print(cout);

        cout<<"\n\n AFTER CNF:\n";
        tree.CNF(tree.root);
        tree.print(cout);





    } catch (exception& e)
    {
        cerr << e.what();
    }

    return 0;
}
