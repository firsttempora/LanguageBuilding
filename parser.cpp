#include "parser.hpp"
#include "lexer.hpp"
#include <ctype.h>

using namespace omega_parser;
using namespace omega_lexer;
ExprAST::~ExprAST() {};

NumberExprAST::NumberExprAST(double val) : Val(val) {};

VariableExprAST::VariableExprAST(const std::string &name) : Name(name) {};

BinaryExprAST::BinaryExprAST(char op, ExprAST *lhs, ExprAST *rhs) : Op(op), LHS(lhs), RHS(rhs) {};

CallExprAST::CallExprAST(const std::string &callee, std::vector<ExprAST*> args): Callee(callee), Args(args) {};

PrototypeExprAST::PrototypeExprAST(const std::string &name, const std::vector<std::string> &args) : Name(name), Args(args) {};

FunctionAST::FunctionAST(PrototypeExprAST * proto, ExprAST * body) : Proto(proto), Body(body) {};

// ****** //
// PARSER //
// ****** //

OmegaParser::OmegaParser(OmegaLexer * lex) : Lex(lex) {
    // Install standard binary operator precedence values
    BinopPrecedence['<'] = 10;
    BinopPrecedence['>'] = 10;
    BinopPrecedence['+'] = 20;
    BinopPrecedence['-'] = 20;
    BinopPrecedence['*'] = 40;
    BinopPrecedence['/'] = 40;
};

void OmegaParser::GetNextToken() {
    curr_tok = Lex -> getTok();
};

int OmegaParser::GetTokPrecedence(char lchar){
    // If not a regular ASCII character, give it minimum precedence
    if(!isascii(lchar)) return -1;
    
    // If not defined in the precedence table, give it minimum precedence
    int tok_prec = BinopPrecedence[lchar];
    if (tok_prec < 0) return -1;
    return tok_prec;
};

FunctionAST * OmegaParser :: ParseTopLevel(){
    // Expressions outside of any function are parsed as if in a zero argument anonymous function
    ExprAST * E = ParseExpression();
    PrototypeExprAST * proto = new PrototypeExprAST("", std::vector<std::string>());
    return new FunctionAST(proto, E);
    /* Currently it seems like with something like 1*2+3*4, the 1*2 will get returned
     * as a function, but then will try to parse the '+' as a primary expression the
     * next time this is called...
     */
};

ExprAST * OmegaParser::ParsePrimary() {
    // This assumes that the lexer has advanced to the token we want to parse
    switch(curr_tok){
        default: 
            throw SyntaxException("Unknown token when expecting an expression!");
            break;
        case Token::tok_identifier:
            return ParseIdentifierExpr();
        case Token::tok_number:
            return ParseNumberExpr();
        case Token::tok_unknown:
            switch(Lex -> return_char){
                default:
                    throw SyntaxException("Unexpected character");
                case '(':
                    return ParseParenExpr();
            }
    }
};

ExprAST * OmegaParser::ParseExpression(){
    ExprAST * lhs = ParsePrimary();
    return ParseBinOpRHS(0, lhs);
};

ExprAST * OmegaParser::ParseBinOpRHS(int expr_prec, ExprAST* lhs){
    // Assume that the LHS has been parsed and now we need to read the 
    // binary operator
    while(true){
        int tok_prec = GetTokPrecedence(Lex -> return_char);
        // Operators not defined in the precedence table get a value of -1
        // so we stop and just return the LHS as the expression, since that
        // means we're not looking at a binary expression.
        // At the same time, if it IS defined, but has lower precedence than
        // the last operator, we also stop because the current expression
        // should be evaluated before going on. E.g. in 2*4+3, if our LHS
        // is 2*4, that needs to be evaulated first, since * comes first in
        // order of operations. OTOH, if the expression was 3+2*4...
        if(tok_prec < expr_prec) return lhs;

        // If we made it this far, we have a higher-precedence binary op
        int op_code = Lex -> return_char;

        GetNextToken();

        // Parse the primary expression after the operator
        ExprAST * rhs = ParsePrimary();

        // ParsePrimary() should leave the lexer pointing to the next token
        // after parsing a primary expression (number, variable, function)
        int next_prec = GetTokPrecedence(Lex -> return_char);
        if(tok_prec < next_prec){
            rhs = ParseBinOpRHS(tok_prec+1, rhs);
        }

        lhs = new BinaryExprAST(op_code, lhs, rhs);
    }
};

ExprAST * OmegaParser::ParseNumberExpr(){
    ExprAST * result = new NumberExprAST(Lex -> numeric_val);
    // We typically choose to have each parse submethod advance
    // the token
    GetNextToken();
    return result;
};

ExprAST * OmegaParser::ParseParenExpr(){
    // Assume that the current token is a '('
    // Advance to the next token, which will be part of the parenthetical expression
    GetNextToken();
    // Unlike the example, I'm actually throwing errors rather than returning null,
    // so we shouldn't need to check if V is null. We may need to do some error 
    // handling though
    //
    // ParseExpression() is the top level parser method that determines if an expression
    // is a binary operation or not and works its way through the RHS of each binary
    // expression present. It should stop upon reaching a ')'
    ExprAST * V = ParseExpression();

    if (Lex -> return_char != ')'){
        throw SyntaxException("Expecting )");
    }

    // Otherwise, we eat the )
    GetNextToken();
    return V;
};


ExprAST * OmegaParser::ParseIdentifierExpr(){
    // This assumes that we've just parsed some kind of identifier
    // which might be a variable name or a function name. We'll 
    // figure out which by whether the next token is a "("
    std::string id_name = Lex -> identifier_str;

    // Advance to the next token: is it a "("?
    GetNextToken();

    if(Lex -> return_char != '(') return new VariableExprAST(id_name);

    // Advance to the first argument, or the close paren if no arguments
    GetNextToken();
    std::vector<ExprAST*> args;
    bool want_arg = true;
    while ( Lex -> return_char != ')' ){
        if (want_arg && Lex -> return_char == ',')
            throw SyntaxException("Expected argument or close paren, got comma");
        else if (want_arg){
            ExprAST * arg = ParseExpression();
            args.push_back(arg);
        }
        else if (!want_arg && Lex -> return_char != ',')
            throw SyntaxException("Expected comma or close paren");

        GetNextToken();
    }

    // Advance past the close paren.
    GetNextToken();
    return new CallExprAST(id_name, args);
}
