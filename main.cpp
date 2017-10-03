#include <iostream>
#include "lexer.hpp"

int main(){
    /* Class var( args ) uses automatic memory, so when var goes out of scope, it gets cleaned up
     * Class var = new Class( args ) will not automatically get cleaned up
     * https://stackoverflow.com/questions/8839943/why-does-the-use-of-new-cause-memory-leaks/8840302#8840302
     */
    OmegaLexer Lex("test.txt");
    char c;
    while(c=Lex.getChar()){
        std::cout << "The next character is: " << c << std::endl;
    }
    return 0;
    std::cout << "Hello world!" << std::endl;
}
