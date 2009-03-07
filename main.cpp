#include "PBJ.h"

int main(int argc, char *argv[])
{
    pANTLR3_UINT8 filename;
    pANTLR3_INPUT_STREAM input;
    pPBJLexer lxr;
    pANTLR3_COMMON_TOKEN_STREAM tstream;
    pPBJParser psr;

    if (argc < 2 || argv[1] == NULL)
        filename = (pANTLR3_UINT8)"./input";
    else
        filename = (pANTLR3_UINT8)argv[1];

    input = antlr3AsciiFileStreamNew(filename);
    if ( input == NULL ) {
        fprintf(stderr, "Failed to open file %s\n", (char *)filename);
        exit(1);
    }

    lxr = PBJLexerNew(input);
    if ( lxr == NULL ) {
        fprintf(stderr, "Unable to create the lexer due to malloc() failure1\n");
        exit(1);
    }

    tstream = antlr3CommonTokenStreamSourceNew(ANTLR3_SIZE_HINT, TOKENSOURCE(lxr));
    if (tstream == NULL) {
	fprintf(stderr, "Out of memory trying to allocate token stream\n");
	exit(1);
    }

    psr = PBJParserNew(tstream);
    if (psr == NULL) {
	fprintf(stderr, "Out of memory trying to allocate parser\n");
	exit(ANTLR3_ERR_NOMEM);
    }

    psr->protocol(psr);

    psr->free(psr);
    psr = NULL;

    tstream->free(tstream);
    tstream = NULL;

    lxr->free(lxr);
    lxr = NULL;

    input->close(input);
    input = NULL;

    return 0;
}
