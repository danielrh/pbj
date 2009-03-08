#include <antlr3.h>
#include "PBJ.h"
#include "PBJParseUtil.h"

void ANTLR3_CDECL freeSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types->free(symtab->types);
}

void initSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types = antlr3HashTableNew(11);
    symtab->enum_values = antlr3HashTableNew(11);
    symtab->free = freeSymbolTable;
}

void defineType(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id) {
    if (symtab == NULL) return;
    symtab->types->put(symtab->types, id->chars, id, NULL);
    //printf("define type \%s\n", id->chars);
}

void defineEnumValue(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING id) {
    if (symtab == NULL) return;
    symtab->enum_values->put(symtab->enum_values, id->chars, id, NULL);
    //printf("define enum value \%s\n", id->chars);
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
