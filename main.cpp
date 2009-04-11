#include "PBJ.h"
extern "C" {
#include "PBJParseUtil.h"
}
#include <sstream>
#include <iostream>
#include "PBJLanguageOutput.hpp"

int main(int argc, char *argv[])
{
    pANTLR3_UINT8 filename;
    pANTLR3_INPUT_STREAM input;
    pPBJLexer lxr;
    pANTLR3_COMMON_TOKEN_STREAM tstream;
    pPBJParser psr,ctx;
    PBJParser_protocol_return     pbjAST;
    if (argc < 2 || argv[1] == NULL)
        filename = (pANTLR3_UINT8)"./input";
    else
        filename = (pANTLR3_UINT8)argv[1];
    const char * outputFilename="output";
    if (argc>=3) {
        outputFilename=argv[2];
    }
    
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

    psr = ctx = PBJParserNew(tstream);
    if (psr == NULL) {
        fprintf(stderr, "Out of memory trying to allocate parser\n");
        exit(ANTLR3_ERR_NOMEM);
    }
    ctx->pPBJParser_NameSpaceTop=NameSpacePush(ctx);
    SCOPE_TOP(NameSpace)->filename=tstream->tstream->tokenSource->strFactory->newRaw(tstream->tstream->tokenSource->strFactory);
    SCOPE_TOP(NameSpace)->filename->append8(SCOPE_TOP(NameSpace)->filename,(const char*)outputFilename);
    SCOPE_TOP(NameSpace)->output=(struct LanguageOutputStruct*)malloc(sizeof(struct LanguageOutputStruct));
    SCOPE_TOP(NameSpace)->output->cs=&std::cerr;
    SCOPE_TOP(NameSpace)->output->cpp=&std::cout;//could open something dependent on filename

    pbjAST=psr->protocol(psr);
    if (psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec) > 0)
    {
        ANTLR3_FPRINTF(stderr, "The parser returned \%d errors, tree walking aborted.\n", psr->pParser->rec->getNumberOfSyntaxErrors(psr->pParser->rec));
 
    }
    else
    {
        pANTLR3_COMMON_TREE_NODE_STREAM    nodes;
        nodes   = antlr3CommonTreeNodeStreamNewTree(pbjAST.tree, ANTLR3_SIZE_HINT); // sIZE HINT WILL SOON BE DEPRECATED!!
        pANTLR3_STRING s = nodes->stringFactory->newRaw(nodes->stringFactory);
        grammarToString(nodes->tnstream,nodes->root,NULL,s);
        FILE*fp=fopen(outputFilename,"w");
        if (s->size>1)
            fwrite(s->chars,s->size-1,1,fp);
        fclose(fp);
        stringFree(s);
        nodes   ->free  (nodes);        nodes   = NULL;
    }
/*
    if (SCOPE_TOP(NameSpace)->output->cpp)
        fclose(SCOPE_TOP(NameSpace)->output->cpp);
    if (SCOPE_TOP(NameSpace)->output->cs)
        fclose(SCOPE_TOP(NameSpace)->output->cs);
*/
    NameSpacePop(ctx);
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
