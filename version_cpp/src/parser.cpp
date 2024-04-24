using namespace std;

#include "parser.hpp"

const char* UnknownToken::what() const noexcept {
      string *str =  new string("Error : Unknown token '" + token.getValue() + " (" + TokensToStr(token.getTokenType()) + ")'");
      return str->c_str();
}

const char* MissingToken::what() const noexcept {
      string *str =  new string("Error : Missing token '" + token + "'");
      return str->c_str();
}

void removeParenthesis(Node *t) {
    if (t->getTokenType() == Token::ClosingParenthesis) t->replaceData(t->getChild());
    else if (t->getTokenType() == Token::OpeningParenthesis) throw MissingToken(")");
    Node *child = t->getChild();
    while (child != nullptr) {
        removeParenthesis(child);
        child = child->getNext();
    }
}

Node *getRootOrParenthesis(Node *tree) {
    if (tree == nullptr ||
            tree->getTokenType() == Token::OpeningParenthesis ||
            tree->getParent() == nullptr ||
            tree->getParent()->getTokenType() == Token::NullRoot)
        return tree;
    return getRootOrParenthesis(tree->getParent());
}

Node *parser(Node *tokenList, bool debug, bool implicitPriority) {

    Node *tree = new Node{Token::NullRoot};
    while (tokenList != nullptr) {
        Token tokenType = tokenList->getTokenType();
        if (tokenType == Token::Number) {
            tree = parseNumber(tree, tokenList);
        }
        else if (tokenType == Token::Name) {
            tree = parseVariable(tree, tokenList);
        }
        else if (tokenType == Token::Plus ||
                tokenType == Token::Minus ||
                tokenType == Token::Times ||
                tokenType == Token::Slash ||
                tokenType == Token::Caret ||
                tokenType == Token::DoubleTimes) {
            tree = parseOperator(tree, tokenList);
        }
        else if (tokenType == Token::OpeningParenthesis) {
            tree = parseOpeningParenthesis(tree, tokenList);
        }
        else if (tokenType == Token::ClosingParenthesis) {
            tree = parseClosingParenthesis(tree, tokenList);
        }
        else {
            throw UnknownToken(*tokenList);
        }
        if (debug) {
            cerr << endl << "Root : " << endl;
            root(tree)->display(cerr);
            cerr << endl;
        }
        tokenList = tokenList->getNext();
    }

    tree = root(tree);
    removeParenthesis(tree);
    deleteNullRoot(tree);
    return tree;
}

Node *parseNumber(Node *tree, Node *token) {
    return getRootOrParenthesis(tree->appendChild(new Number{token->getValue()}));
}

Node *parseVariable(Node *tree, Node *token) {
    return getRootOrParenthesis(tree->appendChild(token->copy()));
}

Node *parseOperator(Node *tree, Node *token) {
    if ((token->getValue() == "-") && (tree == nullptr || tree->getTokenType() == Token::OpeningParenthesis)) {
        tree = tree->appendChild(new Number{"0"});
    }
    if (!isOperator(tree->getTokenType()) || getOperatorPriority(token->getTokenType()) <= getOperatorPriority(tree->getTokenType())) {
        Node *tempNode = token->copy();
        tempNode->appendChild(tree->copy());
        tree->replaceData(tempNode);
        delete tempNode;
    }
    else {
        Node *child;
        child = getLastChild(tree);
        while (isOperator(child->getTokenType()) && getOperatorPriority(token->getTokenType()) > getOperatorPriority(child->getTokenType()))
            child = getLastChild(child);
        Node *tempNode = token->copy();
        tempNode->appendChild(child->copy());
        child->replaceData(tempNode);
        delete tempNode;
        tree = child;
    }
    return tree;
}

Node *parseOpeningParenthesis(Node *tree, Node *token) {
    return tree->appendChild(token->copy());
}

Node *parseClosingParenthesis(Node *tree, Node *token) {
    tree = getRootOrParenthesis(tree);
    if (tree->getTokenType() != Token::OpeningParenthesis) {
        throw MissingToken("(");
    }
    tree->setType(Token::ClosingParenthesis);
    if (tree->getParent() != nullptr && tree->getParent()->getTokenType() != Token::NullRoot) tree = tree->getParent();
    return tree;
}