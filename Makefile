SIRIKATA = "/home/daniel/sirikata"
output: output.pbj.hpp output.pb.h
	gcc -o output test.cpp -Wall -I${SIRIKATA}/dependencies/include	-I${SIRIKATA}/libcore/src -I${SIRIKATA}//dependencies/include -I${SIRIKATA}/dependencies/include/boost-1_35 -I${SIRIKATA}/dependencies/include/boost-1_37
output.pb.h: output.pbj.hpp
	${SIRIKATA}/dependencies/bin/protoc --cpp_out=. output.proto
output.pbj.hpp: pbj output.proto
	./pbj protocol/Test.proto output.proto > output.pbj.hpp
pbj : main.cpp PBJ.h PBJLexer.o PBJParser.o PBJParseUtil.o
	g++ -std=c++98 -static -g2 -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o -lantlr3c || g++ -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib -g2 main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o antlr-3.1.2/lib/libantlr3c.a || g++ -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib -g2 main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o -lantlr3c

PBJLexer.c : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJParser.c : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJLexer.o : PBJLexer.h PBJLexer.c
	gcc -c -g2 -Iantlr-3.1.2/include -I/usr/local/include -o PBJLexer.o PBJLexer.c

PBJParser.o : PBJParser.h PBJParser.c
	gcc -c -g2 -Iantlr-3.1.2/include -I/usr/local/include -o PBJParser.o PBJParser.c

PBJParseUtil.o : PBJParseUtil.h PBJParseUtil.c
	gcc -c -g2 -Iantlr-3.1.2/include -I/usr/local/include -o PBJParseUtil.o PBJParseUtil.c

clean:
	rm -f PBJParseUtil.o PBJLexer.o PBJParser.o PBJLexer.c PBJParser.c main.o