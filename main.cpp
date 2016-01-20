#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>



#include "AST.h"
#include <fstream>
int main()
{

    using namespace std;
    try{
        AST tree;
        auto& OUT = cout;

        stringstream ss("! [ ( a and b ) or ( ! b and c ) ] & [ ( A and B ) or ( C and D ) ]");
        tree.parse(ss, OUT);

        OUT<<"\n\n INITIAL Parsing:\n";
        tree.print(OUT);

        OUT<<"\n\n AFTER atomizeNegation:\n";
        tree.atomizeNegation(tree.root);
        tree.print(OUT);


        OUT<<"\n\n AFTER CNF:\n";
        tree.CNF(tree.root);
        tree.print(OUT);





    } catch (exception& e)
    {
        cerr << e.what();
    }

    return 0;
}
