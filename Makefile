SIRIKATA = "/home/daniel/sirikata"
tpbj: output.pbj.hpp output.pb.h test.cpp pbj.hpp cspbj.exe
	g++ -std=c++98 -Wall -g -o tpbj test.cpp output.pb.cc ${SIRIKATA}/dependencies/protobufs/lib/libprotobuf.a -Wall -I${SIRIKATA}/dependencies/include	-I${SIRIKATA}/libcore/src -I${SIRIKATA}//dependencies/include -I${SIRIKATA}/dependencies/include/boost-1_35 -I${SIRIKATA}/dependencies/include/boost-1_37

cspbj.exe: Output.pbj.cs Output.cs Test.cs pbj.hpp
	${SIRIKATA}/dependencies/installed-mono/bin/gmcs -r:${SIRIKATA}/dependencies/installed-protobufs/lib/Google.ProtocolBuffers.dll Output.pbj.cs Output.cs Test.cs PBJ.cs
Output.cs:output.proto
	${SIRIKATA}/dependencies/installed-protobufs/bin/protoc --csharp_out=. output.proto
output.pb.h: output.pbj.hpp
	${SIRIKATA}/dependencies/installed-protobufs/bin/protoc --cpp_out=. output.proto
output.pbj.cs: pbj output.proto protocol/Test.pbj
	./pbj protocol/Test.pbj output.proto 1> output.pbj.hpp 2> Output.pbj.cs
output.pbj.hpp: pbj output.proto protocol/Test.pbj
	./pbj protocol/Test.pbj output.proto 1> output.pbj.hpp 2> Output.pbj.cs
pbj : main.cpp PBJ.h PBJLexer.o PBJParser.o PBJParseUtil.o
	g++ -std=c++98 -Wall -static -g2 -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o -lantlr3c || g++ -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib -g2 main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o antlr-3.1.2/lib/libantlr3c.a || g++ -o pbj -Iantlr-3.1.2/include -Lantlr-3.1.2/lib -I/usr/local/include -L/usr/local/lib -g2 main.cpp PBJLexer.o PBJParser.o PBJParseUtil.o -lantlr3c

PBJLexer.c : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJParser.c : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJLexer.h : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJParser.h : PBJ.g
	java -jar antlr-3.1.3.jar PBJ.g

PBJLexer.o : PBJLexer.h PBJLexer.c
	gcc -c -g2 -Wall -Iantlr-3.1.2/include -I/usr/local/include -o PBJLexer.o PBJLexer.c

PBJParser.o : PBJParser.h PBJParser.c
	gcc -c -g2 -Wall -Iantlr-3.1.2/include -I/usr/local/include -o PBJParser.o PBJParser.c

PBJParseUtil.o : PBJParseUtil.h PBJParseUtil.cpp
	g++ -c -g2 -Wall -Iantlr-3.1.2/include -I/usr/local/include -o PBJParseUtil.o PBJParseUtil.cpp

clean:
	rm -f PBJParseUtil.o PBJLexer.o PBJParser.o PBJLexer.c PBJParser.c main.o