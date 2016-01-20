#include "AST.h"
#include "RecursiveDescentP.h"
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

void AST::CNF(shared_ptr<node> x){
    if(x->data->type == token::OR){
        if(x->left->data->type == token::AND){
            x->left->data->type = token::OR;
            x->data->type = token::AND;
            auto y = make_shared<node>(token::OR);
            y->left = x->left->right;
            y->right = make_shared<node>(*x->right);
            x->left->right = x->right;
            x->right = y;
        }
        if(x->right->data->type == token::AND){
            x->right->data->type = token::OR;
            x->data->type = token::AND;
            auto y = make_shared<node>(token::OR);
            y->right = x->right->left;
            y->left = make_shared<node>(*x->left);
            x->right->left = x->left;
            x->left = y;
        }
    }

    CNF(x->left);
    CNF(x->right);
}


void AST::parse(std::stringstream& ss){
    RDparser parser(ss);
    root = parser.parseExpression();
}

void AST::parse(std::stringstream& ss, std::ostream& o){
    RDparser parser(ss, o);
    parser.print(o);
    root = parser.parseAnd();

    while(!parser.complete()){
        root = parser.parseRightHalfExpr(root);
    }
}

void AST::atomizeNegation(shared_ptr<node> x){

    if(x->data->negated){
        if(x->data->type == token::OR)
            x->data->type = token::AND;
        else
            x->data->type = token::OR;

        x->data->negated = false;
        if(x->left)
            x->left->data->negated ^= true;
        else TreeError("Syntax Tree Error: missing left child!");

        if(x->right)
            x->right->data->negated ^= true;
        else TreeError("Syntax Tree Error: missing right child!");
    }

    atomizeNegation(x->left);
    atomizeNegation(x->right);


}

void AST::postDestruct(shared_ptr<node> p){
    if (p == nullptr) return;
    postDestruct(p->left);
    postDestruct(p->right);
}


void AST::printRaw(std::ostream& o, std::shared_ptr<node> p, const std::string& prefix, bool isTail){
    if(p == nullptr) return;
    o << prefix + (prefix.empty() ? "    " : (isTail ? "+-- " : "|-- ") );

    p->data->print(o);
    o << std::endl;
    printRaw(o, p->right, prefix + (isTail ? "    " : "|   "), false);
    printRaw(o, p->left, prefix + (isTail ? "    " : "|   "), true);

}
