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

shared_ptr<AST::node> AST::copy(shared_ptr<node> copyFrom){
    if(copyFrom == nullptr) return nullptr;

    shared_ptr<node> newRoot = make_shared<node>(*copyFrom);
    newRoot->left = copy(copyFrom->left);
    newRoot->right = copy(copyFrom->right);
    return newRoot;
}
int step = 0;
void AST::CNF(shared_ptr<node> x){
    if(x == nullptr || !(x->left && x->right)) return;
    if(x->data->type == token::OR){
        if(x->left->data->type == token::AND){
            x->left->data->type = token::OR;
            x->data->type = token::AND;
            auto y = make_shared<node>(token::OR);
            y->left = x->left->right;
            y->right = copy(x->right);
            x->left->right = x->right;
            x->right = y;
        }
        else if(x->right->data->type == token::AND){
            x->right->data->type = token::OR;
            x->data->type = token::AND;
            auto y = make_shared<node>(token::OR);
            y->right = x->right->left;
            y->left = copy(x->left);
            x->right->left = x->left;
            x->left = y;
        }

    }
    if(x->parent && x->parent->data->type == token::OR)
        CNF(x->parent);
    CNF(x->left);
    CNF(x->right);

    std::cout << "\n\n STEP:" << ++step << "\n";
    print(std::cout);
}


void AST::parse(std::istream& ss){
    RDparser parser(ss);
    root = parser.parseExpression();
}

void AST::parse(std::istream& ss, std::ostream& o){
    RDparser parser(ss, o);
    parser.print(o);
    root = parser.parseExpression();
}

void AST::atomizeNegation(shared_ptr<node> x){
    if(x == nullptr || !(x->left && x->right) ) return;
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

    /*std::cout<< "\n\nSTEP " << ++a << ": \n";
    print(std::cout);*/
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
