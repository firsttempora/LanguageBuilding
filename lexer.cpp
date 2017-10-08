#include <fstream>
#include <iostream>
#include <exception>
// for isspace, isalnum, isalpha
#include <ctype.h>
#include "lexer.hpp"

using namespace omega_lexer;

// *********** //
// OMEGA LEXER //
// *********** //

// https://www.uow.edu.au/~lukes/TEXTBOOK/notes-cpp/io/readtextfile.html
OmegaLexer::OmegaLexer(const char* file_name){
    code_file.open(file_name);
    if(!code_file){
        // eventually use string streams to put the file name in the
        // error message
        FileException("OmegaLexer: unable to open file (does it exist?)");
    }
    last_char = ' ';
}

OmegaLexer::~OmegaLexer(void){
    code_file.close();
}

Token OmegaLexer::getTok(void){
    // We want to "eat" any white space, so we start our
    // input character as a space (in the constructor) in 
    // order to guarantee that the while loop runs 

    while(isspace(last_char)){
        last_char = getChar();
    }

    /* Once we've eaten all the white space, it's time to figure
     * out which token we're looking at. Option 1: end of file
     */
    if(last_char == EOF){
        return Token::tok_eof;
    }

    /* Option 2: it's some kind 
     * of identifier, which will start with a letter and then may
     * consist of letters and numbers (and I'll throw in underscores)
     */
    if(isalpha(last_char)){
        // reset identifier_str to start with the first character
        identifier_str = last_char;

        last_char = getChar();
        while(isalnum( last_char ) || last_char == '_'){
            identifier_str += last_char;
            last_char = getChar();
        }

        if (identifier_str == "function") return Token::tok_func;
        else return Token::tok_identifier;
    }

    /* Option 3: a number.
     */
    bool found_decimal = false;
    std::string number_string;
    if(isdigit(last_char) || last_char == '.'){
        number_string = last_char;
        if( last_char == '.' ) found_decimal = true;

        last_char = getChar();
        while(isdigit( last_char )  || last_char  == '.' ){
            if (last_char == '.'){
                if(!found_decimal) found_decimal = true;
                else throw SyntaxException("A decimal number literal cannot have two decimal points");
            }
            number_string += last_char;
            last_char = getChar();
        }

        numeric_val = atof(number_string.c_str());
        return Token::tok_number;
    }
    
    /* Option 4: a comment until the end of the line
     */
    
    if (last_char == '#'){
        do last_char == getChar();
        while (last_char != EOF && last_char != '\n' && last_char != '\r');

        // Unless we've hit the end of the file, there should be another token
        // following this one, so find it because we have to return a token.
        if(last_char != EOF)
            return getTok();
    }

    /* Otherwise, we don't know what it is, so just return it. Positive
     * token values are returned for unspecified characters like this.
     */
    return_char = last_char;
    last_char = getChar(); // we need to advance or we'll be stuck on this character
    return Token::tok_unknown;
}

int OmegaLexer::getChar(void){
    /* Reading one character at a time might not be very efficient,
     * a better way would be to read in a larger chunk and make this
     * code smart enough to iterate over that chunk until the chunk
     * is exhausted then get the next chunk.
     */
    int ch;
    ch = code_file.get();
    return ch;
}

// ************** //
// FILE EXCEPTION //
// ************** //

FileException::FileException(){
    msg_ = "File not found";
}

FileException::FileException(const char * msg){
    msg_ = msg;
}

const char * FileException::what() const throw(){
    return msg_;
}

// ************** //
// SYNTAX EXCEPTION //
// ************** //

SyntaxException::SyntaxException(){
    msg_ = "Syntax not found";
}

SyntaxException::SyntaxException(const char * msg){
    msg_ = msg;
}

const char * SyntaxException::what() const throw(){
    return msg_;
}
