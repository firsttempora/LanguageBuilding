CPPFLAGS = -std=c++11

main.exe: lexer.o parser.o main.cpp
	g++ $(CPPFLAGS) -o main.exe lexer.o main.cpp

lexer.o: lexer.cpp lexer.hpp
	g++ $(CPPFLAGS) -c lexer.cpp

parser.o: parser.cpp lexer.hpp
	g++ $(CPPFLAGS) -c parser.cpp
