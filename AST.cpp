#include "AST.h"

void AST::print(std::ostream& o, token* p, int indent){

    if(p->type > token::OR) { //SHORT FOR "p IS NOT LEAF"
        if(p->right) {
            print(o, p->right, indent+5);
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


    if(p->type >= token::OR) {
        if(p->left) {
            o << std::setw(indent) << ' ' <<" \\\n";
            print(o, p->left, indent+5);
        }
    }
}

void AST::parse(std::stringstream& ss){
    RDparser parser(ss);
    root = parser.parseExpression();
    while(!parser.complete())
        root = parser.parseRightHalfExpr(root);
}

void AST::parse(std::stringstream& ss, std::ostream& o){
    RDparser parser(ss, o);
    root = parser.parseExpression();

    while(!parser.complete()){
        print(std::cout);
        root = parser.parseRightHalfExpr(root);
    }
}

void AST::postDestruct(token *p){
    if (p == nullptr) return;
    if (p->type >= token::OR) postDestruct(p->left); //NOT LEAF
    if (p->type > token::OR) postDestruct(p->right); //NOT UNARY OR LEAF
    delete p;
}
