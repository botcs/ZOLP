#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
using namespace std;
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


//
//GRAMMAR:
//expression -> term or term
//expression -> term
//
//term -> unary and unary
//term -> unary
//
//unary -> not factor
//uanry -> factor
//
//factor -> ( expression )
//factor -> atom
//
//atom -> true
//atom -> false
//atom -> variable

#include <exception>
class ParseError : public std::exception {
    const char* _detail;
    const char* what() const throw(){
        return _detail;
    };
public:
    ParseError(const char* detail) : _detail(detail){}
};

#include <vector>


struct RDparser
{
    token * parseRightHalfExpr(token*);

    token * parseExpression();
    token * parseTerm();
    token * parseUnary();
    token * parseFactor();
    token * parseAtom();

    std::vector<const char*> tokens;
    size_t index = 0;

    const char* getNext(){return tokens[index];}

    bool accept(const char* s){
        if(index > tokens.size()) throw ParseError("Overindexing");
        if(getNext() == s){
            index++;
            return true;
        }
        return false;
    }

    bool complete(){
        return tokens.size() == index;
    }

    RDparser(std::stringstream & Buffer4Parse){
        std::string token;
        while(Buffer4Parse >> token){
            tokens.push_back(token.c_str());
        }
        for(auto a : tokens){
            cout << a;
        }
    }
};

token * RDparser::parseRightHalfExpr(token * leftChild){
    if (accept("or")){
        auto rightChild  = parseTerm();
        auto realOrNode = new token;
        realOrNode->type = token::OR;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
        return realOrNode;
    }
    return leftChild;
}

token * RDparser::parseExpression(){
    auto leftChild = parseTerm();
    if (accept("or")){
        auto rightChild  = parseTerm();
        auto realOrNode = new token;
        realOrNode->type = token::OR;
        realOrNode->left = leftChild;
        realOrNode->right = rightChild;
        return realOrNode;
    }
    return leftChild;
}
token * RDparser::parseTerm(){
    auto leftChild = parseUnary();
    if (accept("and")){
        auto rightChild  = parseUnary();
        auto realAndNode = new token;
        realAndNode->type = token::AND;
        realAndNode->left = leftChild;
        realAndNode->right = rightChild;
        return realAndNode;
    }
    return leftChild;
}
token * RDparser::parseUnary(){
    if(accept("not")){
        auto realNotNode = new token;
        realNotNode->type = token::NOT;
        realNotNode->left = parseUnary();
        return realNotNode;
    }
    return parseFactor();
}
token * RDparser::parseFactor(){
    if(accept("(")){
        auto parenExpr = parseExpression();
        if(accept(")")) return parenExpr;
        else throw ParseError("Missing paren");
    }
    if(accept("true")){
        auto literal = new token;
        literal->type = token::LITERAL;
        literal->litVal = true;
        return literal;
    }
    if(accept("false")){
        auto literal = new token;
        literal->type = token::LITERAL;
        literal->litVal = false;
        return literal;
    }

    auto variable = new token;
    variable->type = token::VARIABLE;
    variable->varName = getNext();
    index++;
    return variable;
}


struct AST //Abstract Syntax Tree
{
    token *root = nullptr;

    void print(std::ostream& o){
        print(o, root);
    }

    void print(std::ostream& o, token* p, int indent = 0){

        if(p->type > 2) { //SHORT FOR "p IS NOT LEAF"
            if(p->right) {
                print(o, p->right, indent+3);
            }
            if (indent) {
                o << std::setw(indent) << ' ';
            }

            if (p->right) o<<" /\n" ;
        }

        if (indent) {
            o << std::setw(indent) << ' ';
        }
        p->print(o);
        o << "\n";


        if(p->type > 1) {
            if(p->left) {
                o << std::setw(indent) << ' ' <<" \\\n";
                print(o, p->left, indent+3);
            }
        }
    }

    void parse(std::stringstream& ss){
        RDparser parser(ss);
        root = parser.parseExpression();
        /*while(!parser.complete())
            root = parser.parseRightHalfExpr(root);
        */

    }

    ~AST(){
        if(root)
            postDestruct(root);
    }

    void postDestruct(token *x){
        if(x == nullptr) return;
        if (x->type > 1) postDestruct(x->left); //NOT LEAF
        if (x->type > 2) postDestruct(x->right); //NOT UNARY OR LEAF
        delete x;
    }
};



int main()
{

    using namespace std;
    try{
        AST tree;
        //rdp.tokens=vector<const char*>{"(", "first", "and", "second", ")",  "and", "third"};
        //rdp.tokens=vector<const char*>{"(", "first", "and", "second", ")",  "and", "not", "not", "true"};
        //rdp.tokens=vector<const char*>{"first", "and", "second", "and", "third"};
        stringstream ss("true and false");
        tree.parse(ss);
        tree.print(cout);

    } catch (exception& e)
    {
        cout << e.what();
    }

    return 0;
}
