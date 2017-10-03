main.exe: lexer.o main.cpp
	g++ -o main.exe lexer.o main.cpp

lexer.o: lexer.cpp lexer.hpp
	g++ -c lexer.cpp
