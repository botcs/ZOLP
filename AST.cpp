#include "AST.h"

void AST::print(std::ostream& o, token* p, int indent){

    if(p->type > 2) { //SHORT FOR "p IS NOT LEAF"
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


    if(p->type > 1) {
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

void AST::postDestruct(token *x){
    if(x == nullptr) return;
    if (x->type > 1) postDestruct(x->left); //NOT LEAF
    if (x->type > 2) postDestruct(x->right); //NOT UNARY OR LEAF
    delete x;
}
