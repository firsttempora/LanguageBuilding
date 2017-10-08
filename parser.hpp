#ifndef PARSER_H
#define PARSER_H

#include <map>
#include <string>
#include <vector>
#include "lexer.hpp"

using namespace omega_lexer;

namespace omega_parser{

/// Base class for all expression nodes
class ExprAST{
    public:
        virtual ~ExprAST();
};

/// Class for numeric literal nodes
class NumberExprAST : public ExprAST {
        double Val;
    public:
        NumberExprAST(double val);
};

/// Class for variable nodes
class VariableExprAST : public ExprAST {
        std::string Name;
    public:
        VariableExprAST(const std::string &name);    
};

/// Class for binary operator nodes
class BinaryExprAST : public ExprAST {
        char Op;
        ExprAST *LHS, *RHS;

    public:
        BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs);
};

/// Class for function calls
class CallExprAST : public ExprAST {
        std::string Callee;
        std::vector<ExprAST*> Args;
    public:
        CallExprAST(const std::string &callee, std::vector<ExprAST*> args);
};

/// Class for function definitions
// These are not derived from ExprAST because they will not be used as expressions?
class PrototypeExprAST{
        std::string Name;
        std::vector<std::string> Args;
    public:
        PrototypeExprAST(const std::string &name, const std::vector<std::string> &args);
};

/// Class for function bodies
// These are not derived from ExprAST because they will not be used as expressions?
class FunctionAST{
        PrototypeExprAST *Proto;
        ExprAST *Body;
    public:
        FunctionAST(PrototypeExprAST * proto, ExprAST * body);
};


/// Primary parser
class OmegaParser{
        OmegaLexer * Lex;
        std::map<char, int> BinopPrecedence;
    public:
        Token curr_tok;

        // use pointer b/c always refer to instances by pointer?
        // const?
        OmegaParser(OmegaLexer * lex);
       
        void GetNextToken(); 
        int GetTokPrecedence(char);
        
        FunctionAST * ParseTopLevel();
        ExprAST * ParsePrimary();
        ExprAST * ParseExpression();
        ExprAST * ParseBinOpRHS(int, ExprAST*);
        ExprAST * ParseNumberExpr();
        ExprAST * ParseParenExpr();
        ExprAST * ParseIdentifierExpr();

};
}
#endif
