#ifndef LEXER_H
#define LEXER_H

#include <fstream>
#include <iostream>
#include <exception>
#include <string>

namespace omega_lexer {

class FileException : public std::exception {
    const char * msg_;
    
    public:
        FileException();
        FileException(const char * msg);
        virtual const char * what() const throw();
};

class SyntaxException : public std::exception {
    const char * msg_;
    
    public:
        SyntaxException();
        SyntaxException(const char * msg);
        virtual const char * what() const throw();
};

// https://stackoverflow.com/a/12183377 the first comment: "enum class" makes
// it so that enums get their own scope, and the elements aren't dumped into
// the global namespace
enum class Token {
    tok_eof = EOF,
    tok_func = -2,
    tok_identifier = -10,
    tok_number = -20,
    tok_unknown = 1
};

class OmegaLexer {
        bool UseCIN;
    public:
        int last_char;
        int return_char;
        std::string identifier_str;
        double numeric_val;

        // Constructors and destructor
        OmegaLexer(void); // to read from cin
        OmegaLexer(const char*);
        ~OmegaLexer();

        // Main file parsing functions
        Token getTok();
        int getChar(); // return type of int b/c EOF is -1 (char only guaranteed to support 0 to 127)

    private:
        std::ifstream code_file;
};

}
#endif
