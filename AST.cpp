#include "AST.h"

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

void AST::printRaw(std::ostream& o, token* p, int indent){
    auto i = indent;
    while(i){
        if (i > 3 )
             o << "|  ";
        else o << "|..";
        i-=3;
    }
    p->print(o);
    o << "\n";
    if(p->isBinary()){
        printRaw(o, p->left, indent + 3);
        printRaw(o, p->right, indent + 3);
    } else if (p->isUnary())
        printRaw(o, p->child, indent + 3);

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

    if(x->type > token::FALSE){ //X IS NOT LEAF
        if(x->type == token::NOT && x->left->type > token::FALSE){

        }
    } else return;
}

void AST::postDestruct(token *p){
    if (p == nullptr) return;
    if (p->type >= token::OR) postDestruct(p->left); //NOT LEAF
    if (p->type > token::OR) postDestruct(p->right); //NOT UNARY OR LEAF
    delete p;
}
