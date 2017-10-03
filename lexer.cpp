#include <fstream>
#include <iostream>
#include <exception>

#include "lexer.hpp"

OmegaLexer::OmegaLexer(const char* file_name){
    code_file.open(file_name);
}

OmegaLexer::~OmegaLexer(void){
    code_file.close();
}

char OmegaLexer::getChar(void){
    /* Reading one character at a time might not be very efficient,
     * a better way would be to read in a larger chunk and make this
     * code smart enough to iterate over that chunk until the chunk
     * is exhausted then get the next chunk.
     */
    char ch;
    code_file >> ch;
    return ch;
}



FileException::FileException(){
    msg_ = "File not found";
}

FileException::FileException(const char * msg){
    msg_ = msg;
}

const char * FileException::what() const throw(){
    return msg_;
}
