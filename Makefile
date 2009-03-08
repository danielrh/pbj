
pbj : main.cpp PBJ.h PBJLexer.o PBJParser.o PBJParseUtil.o
	g++ -m32 -o pbj -I/usr/local/include -L/usr/local/lib -lantlr3c main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o

PBJLexer.o : PBJLexer.h PBJLexer.c
	gcc -m32 -c -I/usr/local/include -o PBJLexer.o PBJLexer.c

PBJParser.o : PBJParser.h PBJParser.c
	gcc -m32 -c -I/usr/local/include -o PBJParser.o PBJParser.c

PBJParseUtil.o : PBJParseUtil.h PBJParseUtil.c
	gcc -m32 -c -I/usr/local/include -o PBJParseUtil.o PBJParseUtil.c
