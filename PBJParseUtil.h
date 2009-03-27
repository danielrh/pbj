#ifndef _PBJ_PARSE_UTIL_H_
#define _PBJ_PARSE_UTIL_H_

#undef	SCOPE_TYPE
#undef	SCOPE_STACK
#undef	SCOPE_TOP
#define	SCOPE_TYPE(scope)   pPBJParser_##scope##_SCOPE
#define SCOPE_STACK(scope)  pPBJParser_##scope##Stack
#define	SCOPE_TOP(scope)    ctx->pPBJParser_##scope##Top
#define	SCOPE_SIZE(scope)			(ctx->SCOPE_STACK(scope)->size(ctx->SCOPE_STACK(scope)))
#define SCOPE_INSTANCE(scope, i)	(ctx->SCOPE_STACK(scope)->get(ctx->SCOPE_STACK(scope),i))
typedef struct LanguageOutputStruct {
    FILE * cpp;
    FILE * cs;
} LanguageOutput;

void initSymbolTable(SCOPE_TYPE(Symbols) symtab);
void initNameSpace(SCOPE_TYPE(NameSpace) symtab);

void definePackage(SCOPE_TYPE(NameSpace) symtab, pANTLR3_STRING id);
void defineImport(SCOPE_TYPE(NameSpace) symtab, pANTLR3_STRING filename);
void defineType(SCOPE_TYPE(NameSpace) ns, SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id);
void defineEnumValue(SCOPE_TYPE(NameSpace), SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING enumName, pANTLR3_LIST enumList, pANTLR3_STRING id,  pANTLR3_STRING value);
void defineFlagValue(SCOPE_TYPE(NameSpace), SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING flagName, pANTLR3_LIST flagList, pANTLR3_STRING id,  pANTLR3_STRING value);
void defineEnum(SCOPE_TYPE(NameSpace), SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST enumList);
void defineFlag(SCOPE_TYPE(NameSpace), SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST flagLis, unsigned int bits);
void defineField(SCOPE_TYPE(NameSpace),SCOPE_TYPE(Symbols), pANTLR3_STRING type, pANTLR3_STRING name, pANTLR3_STRING value, int notRepeated, int multiplicitive_advanced_type);



void ANTLR3_CDECL freeSymbolTable(SCOPE_TYPE(Symbols) symtab);
void stringFree(void *s);
void grammarToString(pANTLR3_TREE_NODE_STREAM node_stream, pANTLR3_BASE_TREE base, pANTLR3_BASE_TREE stop, pANTLR3_STRING bufOutput);
pANTLR3_STRING stringDup(pANTLR3_STRING s);
pANTLR3_STRING defaultValuePreprocess(SCOPE_TYPE(NameSpace),SCOPE_TYPE(Symbols), pANTLR3_STRING type, pANTLR3_STRING value);
ANTLR3_BOOLEAN isTypeName(pPBJParser ctx, pANTLR3_UINT8 name);
#endif //_PBJ_PARSE_UTIL_H_
