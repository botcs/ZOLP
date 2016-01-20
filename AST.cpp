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

void AST::CNF(token*x){
    //std::cout<<x->type<<"ASD\n";
    if(!x->isBinary()) return;
    if(x->type == token::OR){
        if(x->left->type == token::AND){
            x->left->type = token::OR;
            x->type = token::AND;
            auto y = new token(token::OR);
            y->left = x->left->right;
            y->right = new token(*x->right);
            x->left->right = x->right;
            x->right = y;
        }
        if(x->right->type == token::AND){
            x->right->type = token::OR;
            x->type = token::AND;
            auto y = new token(token::OR);
            y->right = x->right->left;
            y->left = new token(*x->left);
            x->right->left = x->left;
            x->left = y;
        }
    }

    CNF(x->left);
    CNF(x->right);
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
