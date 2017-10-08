#include <iostream>
#include <memory> //for unique_ptr 
#include "lexer.hpp"
#include "omega_utils.hpp"

using namespace omega_lexer;
using namespace omega_utils; // as_integer
using namespace std;

int main(int argc, char *argv[]){
    unique_ptr<OmegaLexer> Lex;
    if( argc == 1){
        Lex = std::unique_ptr<OmegaLexer>(new OmegaLexer());
    }
    else if( argc == 2){
        /* Class var( args ) uses automatic memory, so when var goes out of scope, it gets cleaned up
         * Class var = new Class( args ) will not automatically get cleaned up
         * https://stackoverflow.com/questions/8839943/why-does-the-use-of-new-cause-memory-leaks/8840302#8840302
         */
        Lex = unique_ptr<OmegaLexer>(new OmegaLexer(argv[1]));;
    }
        
    int tok;
    while((tok = as_integer(Lex -> getTok())) != EOF){
        cout << "Token = " << tok << endl;
        if(tok == as_integer(Token::tok_number))
            cout << "   Number is " << Lex -> numeric_val << endl;
        else if(tok >= 0)
            cout << "   Unspecified token is " << static_cast<char>(Lex -> return_char) << endl;
        else
            cout << "   Identifier is " << Lex -> identifier_str << endl;
    }
}
