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

void ANTLR3_CDECL freeSymbolTable(SCOPE_TYPE(Symbols) symtab);
void initSymbolTable(SCOPE_TYPE(Symbols) symtab);
void defineType(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id);
void defineEnumValue(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id);
ANTLR3_BOOLEAN isTypeName(pPBJParser ctx, pANTLR3_UINT8 name);

#endif //_PBJ_PARSE_UTIL_H_
