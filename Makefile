
pbj : main.cpp PBJ.h PBJLexer.h PBJLexer.c PBJParser.h PBJParser.c PBJ.tokens
	g++ -m32 -o pbj -I/usr/local/include -L/usr/local/lib -lantlr3c main.cpp PBJLexer.c PBJParser.c
