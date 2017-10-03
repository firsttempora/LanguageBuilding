#include <fstream>
#include <iostream>
#include <exception>

class FileException : public std::exception {
    const char * msg_;
    FileException();
    FileException(const char * msg);
//    virtual const char * what() const throw();
    virtual const char * what() const throw();
};

// eventually this should go into a header file
class OmegaLexer {
    public:
        OmegaLexer(const char*);
        ~OmegaLexer();
        char getChar();

    private:
        std::ifstream code_file;
};
