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

void definePackage(SCOPE_TYPE(NameSpace) symtab, pANTLR3_STRING id) {
    symtab->package=id->factory->newRaw(id->factory);
    symtab->package->appendS(symtab->package,id);
}

void stringFree(void* s) {
    pANTLR3_STRING id=(pANTLR3_STRING)s;
    if (id) {
        id->factory->destroy(id->factory,id);
    }
}
pANTLR3_STRING stringDup(pANTLR3_STRING s) {
    pANTLR3_STRING retval=s->factory->newRaw(s->factory);
    retval->appendS(retval,s);
    return retval;
}

void defineImport(SCOPE_TYPE(NameSpace) symtab, pANTLR3_STRING filename) {
    pANTLR3_STRING s=filename->factory->newRaw(filename->factory);
    s->appendS(s,filename);
    symtab->imports->add(symtab->imports,s,&stringFree);
    
}

void defineType(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id) {
    if (symtab == NULL) return;
    symtab->types->put(symtab->types, id->chars, id, NULL);
}

void defineEnum(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    if (symtab == NULL) return;
    defineType(ns, symtab,id);
}
void defineEnumValue(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING enumName, pANTLR3_LIST enumValues, pANTLR3_STRING id, pANTLR3_STRING value) {
    if (symtab == NULL) return;
    symtab->enum_values->put(symtab->enum_values, id->chars, value, NULL);
}
void defineFlag(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST flagValues, unsigned int flagBits) {
    unsigned int* bits=(unsigned int *)malloc(sizeof(unsigned int));
    *bits=flagBits;
    if (symtab == NULL) return;
    defineType(ns, symtab, id);

    symtab->flag_sizes->put(symtab->flag_sizes,id->chars,bits,NULL);
}

void defineFlagValue(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING flagName, pANTLR3_LIST flagValues, pANTLR3_STRING id, pANTLR3_STRING value) {
    if (symtab == NULL) return;//FIXME
    symtab->flag_values->put(symtab->enum_values, id->chars, id, NULL);
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

pANTLR3_STRING defaultValuePreprocess(SCOPE_TYPE(NameSpace) ns,SCOPE_TYPE(Symbols) sym, pANTLR3_STRING type, pANTLR3_STRING value){
    return stringDup(value);
}
void defineField(SCOPE_TYPE(NameSpace) nameSpace,SCOPE_TYPE(Symbols) symbols, pANTLR3_STRING type, pANTLR3_STRING name, pANTLR3_STRING value, int notRepeated, int multiplicitive_advanced_type){
    
}
