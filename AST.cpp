#include "AST.h"
#include <exception>
#include <algorithm>

class TreeError : public std::exception {
    const char* _detail;
    const char* what() const throw(){
        return _detail;
    };
public:
    TreeError(const std::string& detail) : _detail(detail.c_str()){}
};

int a = 0;
void AST::CNF(token*x){
    ++a;
    //std::cout<<x->type<<"ASD\n";
    if(!x->isBinary()) return;
    if(x->type == token::OR){
        if(x->left->type == token::AND){
            std::swap(x->type, x->left->type);
            auto y = new token(token::OR);
            y->left = x->left->right;
            y->right = x->right;
            x->left->right = x->right;
            x->right = y;
        }
        else if(x->right->type == token::AND){
            std::swap(x->type, x->right->type);
            auto y = new token(token::OR);
            y->right = x->right->left;
            y->left = x->left;
            x->right->left = x->left;
            x->left = y;
        }
    }

    std::cout << "\n\nSTEP " << a << ": \n";
    print(std::cout);

    CNF(x->left);
    CNF(x->right);
}


void AST::printFancy(std::ostream& o, token* p, int indent){
    if(p->isBinary()) {
        if(p->right) {
            printFancy(o, p->right, indent+5);
        }
        if (indent) {
            o << std::setw(indent) << ' ';
        }

        if (p->right) o<<" /\n" ;
    }

    if (indent) o << std::setw(indent) << ' ';
    p->print(o);
    o << "\n";

    if(p->isBinary()) {
        if(p->left) {
            o << std::setw(indent) << ' ' <<" \\\n";
            printFancy(o, p->left, indent+5);
        }
    } else if (p->isUnary()){
        if(p->child){
            o << std::setw(indent) << ' ' <<" \\\n";
            printFancy(o, p->child, indent+5);
        }
    }

}


void AST::parse(std::stringstream& ss){
    RDparser parser(ss);
    root = parser.parseAnd();
    while(!parser.complete())
        root = parser.parseRightHalfExpr(root);
}

void AST::parse(std::stringstream& ss, std::ostream& o){
    RDparser parser(ss, o);
    parser.print(o);
    root = parser.parseAnd();

    while(!parser.complete()){
        root = parser.parseRightHalfExpr(root);
    }
}

void AST::atomizeNegation(token* x){

    if(!x->isAtom()){ //X IS NOT LEAF

        if(x->isBinary()){
            if(x->negated){
                if(x->type == token::OR)
                    x->type = token::AND;
                else
                    x->type = token::OR;

                x->negated = false;
                if(x->left)
                    x->left->negated ^= true;
                else TreeError("Syntax Tree Error: missing left child!");

                if(x->right)
                    x->right->negated ^= true;
                else TreeError("Syntax Tree Error: missing right child!");
            }

            atomizeNegation(x->left);
            atomizeNegation(x->right);
        }
//         else if(x->isUnary()){
//
//        }
        ///BECAME OBSOLETE AFTER CORRECTING PARSER GRAMMAR
        ///excluding 'NOT' nodes in tree representation

    }

}

void AST::postDestruct(token *p){
    if (p == nullptr) return;
    if (p->type >= token::OR) postDestruct(p->left); //NOT LEAF
    if (p->type > token::OR) postDestruct(p->right); //NOT UNARY OR LEAF
    delete p;
}


void AST::printRaw(std::ostream& o, token* p, const std::string& prefix, bool isTail){

    o << prefix + (prefix.empty() ? "    " : (isTail ? "+-- " : "|-- ") );

    p->print(o);
    o << std::endl;
    if(p->isBinary()){
        printRaw(o, p->right, prefix + (isTail ? "    " : "|   "), false);
        printRaw(o, p->left, prefix + (isTail ? "    " : "|   "), true);

    }
}
