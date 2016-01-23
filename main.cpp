#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>



#include "AST.h"
#include <fstream>
#include <ctime>
int main()
{

    using namespace std;
    try{
        AST tree;
        auto& OUT = cout;

        stringstream ss("A or ( (X and Y) or (W and Z) )");
        auto t = clock();
        tree.parse(ss, OUT);
        cout << clock() - t;

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
