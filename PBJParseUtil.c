#include <antlr3.h>
#include "PBJ.h"
#include "PBJParseUtil.h"

void ANTLR3_CDECL freeSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types->free(symtab->types);
    symtab->flag_sizes->free(symtab->flag_sizes);
    symtab->flag_values->free(symtab->flag_values);
    symtab->enum_values->free(symtab->enum_values);
}
void ANTLR3_CDECL freeNameSpace(SCOPE_TYPE(NameSpace) symtab) {
    symtab->imports->free(symtab->imports);
    fclose(symtab->output->cpp);
}
void initNameSpace(SCOPE_TYPE(NameSpace) symtab) {
    symtab->output=(struct LanguageOutputStruct*)malloc(sizeof(struct LanguageOutputStruct));
    symtab->output->cs=NULL;
    symtab->output->cpp=stdout;
    symtab->package=NULL;
    symtab->imports = antlr3ListNew(1);
    symtab->free=freeNameSpace;
}
void initSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types = antlr3HashTableNew(11);
    symtab->flag_sizes = antlr3HashTableNew(11);
    symtab->flag_values = antlr3HashTableNew(11);
    symtab->enum_values = antlr3HashTableNew(11);
    symtab->free = freeSymbolTable;
}

void definePackage(pPBJParser ctx, pANTLR3_STRING id) {    
    SCOPE_TOP(NameSpace)->package=id->factory->newRaw(id->factory);
    SCOPE_TOP(NameSpace)->package->appendS(SCOPE_TOP(NameSpace)->package,id);
}

pANTLR3_STRING stringDup(pANTLR3_STRING s) {
    pANTLR3_STRING retval=s->factory->newRaw(s->factory);
    retval->appendS(retval,s);
    return retval;
}

void stringFree(void* s) {
    pANTLR3_STRING id=(pANTLR3_STRING)s;
    if (id) {
        id->factory->destroy(id->factory,id);
    }
}

void defineImport(pPBJParser ctx, pANTLR3_STRING filename) {

    pANTLR3_STRING s=filename->factory->newRaw(filename->factory);
    s->appendS(s,filename);
    SCOPE_TOP(NameSpace)->imports->add(SCOPE_TOP(NameSpace)->imports,s,&stringFree);
    
}

void defineType(pPBJParser ctx, pANTLR3_STRING id) {
    if (SCOPE_TOP(Symbols) == NULL) return;
    SCOPE_TOP(Symbols)->types->put(SCOPE_TOP(Symbols)->types, id->chars, id, NULL);
}


ANTLR3_BOOLEAN isTypeName(pPBJParser ctx, pANTLR3_UINT8 name) {
    int i;
    for (i = (int)SCOPE_SIZE(Symbols)-1 ; i >= 0; i--) {
        pANTLR3_HASH_TABLE symtab;
        pANTLR3_STRING symbol;
        SCOPE_TYPE(Symbols) symScope;

        symScope = (SCOPE_TYPE(Symbols))SCOPE_INSTANCE(Symbols, i);
        symtab = (pANTLR3_HASH_TABLE) symScope->types;
        symbol = (pANTLR3_STRING) (symtab->get(symtab, (void *)name));

        if (symbol != NULL)
            return ANTLR3_TRUE;
    }
    return ANTLR3_FALSE;
}

void grammarToString	(pANTLR3_TREE_NODE_STREAM tns, pANTLR3_BASE_TREE p, pANTLR3_BASE_TREE stop, pANTLR3_STRING buf)
{

	ANTLR3_UINT32   n;
	ANTLR3_UINT32   c;

	if	(!p->isNilNode(p) )
	{
		pANTLR3_STRING	text;

		text	= p->toString(p);
        if (text == NULL) {
            pANTLR3_COMMON_TOKEN tok=((pANTLR3_COMMON_TREE)(p->super))->token;
            if (tok->strFactory==NULL) {
                tok->strFactory=buf->factory;
                text = tok->getText(((pANTLR3_COMMON_TREE)(p->super))->token);
            }
        }

		if  (text == NULL)
		{
			text = tns->ctns->stringFactory->newRaw(tns->ctns->stringFactory);

			text->addc	(text, ' ');
			text->addi	(text, p->getType(p));
		}

		buf->appendS(buf, text);
	}

	if	(p == stop)
	{
		return;		/* Finished */
	}

	n = p->getChildCount(p);

	if	(n > 0 && ! p->isNilNode(p) )
	{
		buf->addc   (buf, ' ');
		buf->addi   (buf, ANTLR3_TOKEN_DOWN);
	}

	for	(c = 0; c<n ; c++)
	{
		pANTLR3_BASE_TREE   child;

		child = (pANTLR3_BASE_TREE)p->getChild(p, c);
		grammarToString(tns, child, stop, buf);
	}

	if	(n > 0 && ! p->isNilNode(p) )
	{
		buf->addc   (buf, ' ');
		buf->addi   (buf, ANTLR3_TOKEN_UP);
	}
}
static void openNamespace(pPBJParser ctx) {
    pANTLR3_STRING substr;
    pANTLR3_STRING rest=NULL;
    if (SCOPE_TOP(NameSpace)->package) {
        char *where=strchr(SCOPE_TOP(NameSpace)->package->chars,'.');
        if (where) {
            substr=SCOPE_TOP(NameSpace)->package->subString(SCOPE_TOP(NameSpace)->package,0,where-(char*)SCOPE_TOP(NameSpace)->package->chars);
            rest=SCOPE_TOP(NameSpace)->package->subString(SCOPE_TOP(NameSpace)->package,where+1-(char*)SCOPE_TOP(NameSpace)->package->chars,SCOPE_TOP(NameSpace)->package->size);        
        }else {
            substr=stringDup(SCOPE_TOP(NameSpace)->package);
        }
        do {
            fprintf(SCOPE_TOP(NameSpace)->output->cpp,"namespace %s {\n",substr->chars);
            stringFree(substr);
            substr=NULL;
            if (rest) {
                where=strstr(rest->chars,".");
                if (where) {
                    pANTLR3_STRING toBeFreed=rest;
                    substr=rest->subString(rest,0,where-(char*)rest->chars);
                    rest=rest->subString(rest,where-(char*)rest->chars,rest->size);
                    stringFree(toBeFreed);
                }else {
                    substr=rest;
                    rest=NULL;
                }
            }
        }while (substr);
    }
}
static void closeNamespace(pPBJParser ctx) {
    if (SCOPE_TOP(NameSpace)->package) {
        size_t stringSize=SCOPE_TOP(NameSpace)->package->size;
        size_t i;
        if (SCOPE_TOP(NameSpace)->output->cpp) {
            fprintf (SCOPE_TOP(NameSpace)->output->cpp,"}\n");
            for (i=0;i<stringSize;++i) {       
                if (SCOPE_TOP(NameSpace)->package->chars[i]=='.') {
                    fprintf (SCOPE_TOP(NameSpace)->output->cpp,"}\n");
                }
            }
        }
    }
}
pANTLR3_STRING defaultValuePreprocess(pPBJParser ctx, pANTLR3_STRING type, pANTLR3_STRING value){
    return stringDup(value);
}
#define CPPFP SCOPE_TOP(NameSpace)->output->cpp 
static void sendTabs(pPBJParser ctx,int offset) {
    int num=SCOPE_SIZE(Symbols)+offset-1;
    int i;
    for (i=0;i<num;++i) {
        fprintf(CPPFP,"    ");
    }
}
void defineMessage(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    if (CPPFP) {
        sendTabs(ctx,1);
        fprintf(CPPFP,"class %s : protected _PBJ_Internal::%s {\n",id->chars,id->chars);
        sendTabs(ctx,1);
        fprintf(CPPFP,"public:\n",id->chars,id->chars);
    }
}
void defineExtension(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    if (CPPFP) {
        sendTabs(ctx,1);
        fprintf(CPPFP,"class %sExtend : public %s {\n",id->chars,id->chars);
        sendTabs(ctx,1);
        fprintf(CPPFP,"public:\n",id->chars,id->chars);
    }
}
void defineField(pPBJParser ctx, pANTLR3_STRING type, pANTLR3_STRING name, pANTLR3_STRING value, int notRepeated, int multiplicitive_advanced_type){
}
void printEnum(pPBJParser ctx, int offset, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    int enumSize=enumValues->size(enumValues);
    int i;
    sendTabs(ctx,1);
    fprintf(CPPFP,"enum %s {\n",id->chars);
    for (i=0;i<enumSize;i+=2) {
        pANTLR3_STRING enumVal=((pANTLR3_STRING)(enumValues->get(enumValues,i)));
        sendTabs(ctx,2);
        fprintf(CPPFP,"%s=_PBJ_Internal::%s%s",
                enumVal->chars,
                enumVal->chars,
                (i+2==enumSize?"\n":",\n"));            
    }
    sendTabs(ctx,1);
    fprintf(CPPFP,"};\n");
}
void defineEnum(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    if (SCOPE_TOP(Symbols) == NULL) return;
    defineType(ctx,id);
    if (CPPFP) {
        printEnum(ctx,1,id,enumValues);
    }
        
}
void defineEnumValue(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING enumName, pANTLR3_LIST enumValues, pANTLR3_STRING id, pANTLR3_STRING value) {
    if (SCOPE_TOP(Symbols) == NULL) return;
    SCOPE_TOP(Symbols)->enum_values->put(SCOPE_TOP(Symbols)->enum_values, id->chars, value, NULL);
    enumValues->put(enumValues,enumValues->size(enumValues),id,stringFree);
    enumValues->put(enumValues,enumValues->size(enumValues),value,stringFree);
}
void defineFlag(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST flagValues, unsigned int flagBits) {
    unsigned int* bits=(unsigned int *)malloc(sizeof(unsigned int));
    *bits=flagBits;
    if (SCOPE_TOP(Symbols) == NULL) return;
    defineType(ctx, id);
    if (CPPFP) {
        printEnum(ctx,1,id,flagValues);
    }
    SCOPE_TOP(Symbols)->flag_sizes->put(SCOPE_TOP(Symbols)->flag_sizes,id->chars,bits,NULL);
}

void defineFlagValue(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING flagName, pANTLR3_LIST flagValues, pANTLR3_STRING id, pANTLR3_STRING value) {
    if (SCOPE_TOP(Symbols) == NULL) return;//FIXME
    SCOPE_TOP(Symbols)->flag_values->put(SCOPE_TOP(Symbols)->enum_values, id->chars, id, NULL);
    flagValues->put(flagValues,flagValues->size(flagValues),id,stringFree);
    flagValues->put(flagValues,flagValues->size(flagValues),value,stringFree);
}
void defineMessageEnd(pPBJParser ctx, pANTLR3_STRING id){
    if (CPPFP) {
        sendTabs(ctx,0);
        fprintf(CPPFP,"};\n");
    }
    closeNamespace(ctx);
}

void defineExtensionEnd(pPBJParser ctx, pANTLR3_STRING id){
    if (CPPFP) {
        sendTabs(ctx,0);
        fprintf(CPPFP,"};\n");
    }
    closeNamespace(ctx);
}
    
