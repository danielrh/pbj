#include <antlr3.h>
#include "PBJ.h"
#include "PBJParseUtil.h"
#include <assert.h>
void  freeSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types->free(symtab->types);
    symtab->flag_sizes->free(symtab->flag_sizes);
    symtab->enum_sizes->free(symtab->enum_sizes);
    symtab->flag_values->free(symtab->flag_values);
    symtab->flag_all_on->free(symtab->flag_all_on);
    symtab->enum_values->free(symtab->enum_values);
    if (symtab->message) {
        stringFree(symtab->message);
    }
}
void  freeNameSpace(SCOPE_TYPE(NameSpace) symtab) {
    symtab->imports->free(symtab->imports);
    fclose(symtab->output->cpp);
}
void  initNameSpace(pPBJParser ctx, SCOPE_TYPE(NameSpace) symtab) {
    if (SCOPE_SIZE(NameSpace)>1) {
        SCOPE_TYPE(NameSpace) lowerNamespace;
        int scope_size=SCOPE_SIZE(NameSpace)-2;
        lowerNamespace=(SCOPE_TYPE(NameSpace) ) (SCOPE_INSTANCE(NameSpace,scope_size));
        symtab->filename=stringDup(lowerNamespace->filename);
        symtab->output=(struct LanguageOutputStruct*)malloc(sizeof(struct LanguageOutputStruct));
        memcpy(symtab->output,((SCOPE_TYPE(NameSpace) )SCOPE_INSTANCE(NameSpace,SCOPE_SIZE(NameSpace)-2))->output,sizeof(struct LanguageOutputStruct));
    }
    if (symtab->output->cpp) {
        char lst;
        if (symtab->filename->len>6) {
            lst=symtab->filename->chars[symtab->filename->len-6];
            assert(lst=='.');
            symtab->filename->chars[symtab->filename->len-6]='\0';
        }
        fprintf(symtab->output->cpp,"#include \"pbj.hpp\"\n",symtab->filename->chars);
        fprintf(symtab->output->cpp,"#include \"%s.pb.h\"\n",symtab->filename->chars);
        if (symtab->filename->len>6) {
            symtab->filename->chars[symtab->filename->len-6]=lst;
        }
    }
    symtab->package=NULL;
    symtab->imports = antlr3ListNew(1);
    symtab->free=freeNameSpace;
    
}
void  initSymbolTable(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, int isExtension) {
    symtab->message=NULL;
    symtab->types = antlr3HashTableNew(11);
    symtab->flag_all_on = antlr3HashTableNew(11);
    symtab->flag_sizes = antlr3HashTableNew(11);
    symtab->enum_sizes = antlr3HashTableNew(11);
    symtab->flag_values = antlr3HashTableNew(11);
    symtab->enum_values = antlr3HashTableNew(11);
    if (messageName&&symtab->message==NULL&&!isExtension) {
        symtab->message=stringDup(messageName);
    }
    symtab->free = freeSymbolTable;
}

void  definePackage(pPBJParser ctx, pANTLR3_STRING id) {    
    
    SCOPE_TOP(NameSpace)->package=stringDup(id);

}

pANTLR3_STRING  stringDup(pANTLR3_STRING s) {
    pANTLR3_STRING retval=s->factory->newPtr(s->factory,s->chars,s->len);
    return retval;
}

void  stringFree(void* s) {
    pANTLR3_STRING id=(pANTLR3_STRING)s;
    if (id) {
        id->factory->destroy(id->factory,id);
    }
}

#define CPPFP SCOPE_TOP(NameSpace)->output->cpp 
void defineImport(pPBJParser ctx, pANTLR3_STRING filename) {

    pANTLR3_STRING s=filename->factory->newRaw(filename->factory);
    s->appendS(s,filename);
    SCOPE_TOP(NameSpace)->imports->add(SCOPE_TOP(NameSpace)->imports,s,&stringFree);
    if (CPPFP) {
        char lst;
        if (s->len>6) {
            lst=s->chars[s->len-6];
            assert(lst=='.');
            s->chars[s->len-6]='\0';
        }
        fprintf(CPPFP,"#include \"%s.pbj.hpp\"\n",s->chars);
        if (s->len>6)
            s->chars[s->len-6]=lst;
    }

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
static char* stringChar(pANTLR3_STRING str, ANTLR3_UINT8 searchme) {
    int i;
    for (i=0;i<str->len;++i) {
        if (str->chars[i]==searchme) {
            return (char*)&str->chars[i];
        }
    }
    return NULL;
}
static void openNamespace(pPBJParser ctx) {
    pANTLR3_STRING substr;
    pANTLR3_STRING rest=NULL;
    if (SCOPE_TOP(NameSpace)->package&&SCOPE_SIZE(Symbols)<2) {
        char *where=stringChar(SCOPE_TOP(NameSpace)->package,'.');
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
                where=stringChar(rest,'.');
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
    if (SCOPE_TOP(NameSpace)->package&&SCOPE_SIZE(Symbols)<=2) {
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

static void sendTabs(pPBJParser ctx,int offset) {
    int num=SCOPE_SIZE(Symbols)+offset-1;
    int i;
    for (i=0;i<num;++i) {
        fprintf(CPPFP,"    ");
    }
}

static void sendCppMessageLevels(pPBJParser ctx, FILE *fp) {
    int i;
    for (i=0;i+1<SCOPE_SIZE(Symbols);++i) {
        if (((SCOPE_TYPE(Symbols))(SCOPE_INSTANCE(Symbols,i)))->message) {
            fprintf(fp,"%s::",((SCOPE_TYPE(Symbols))SCOPE_INSTANCE(Symbols,i))->message->chars);
        }
    }
}

void defineMessage(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    if (CPPFP) {
        SCOPE_TOP(Symbols)->message=stringDup(id);
        sendTabs(ctx,1);
        fprintf(CPPFP,"class %s : protected _PBJ_Internal::",id->chars);
        sendCppMessageLevels(ctx,CPPFP);
        fprintf(CPPFP,"%s {\n",id->chars);
        sendTabs(ctx,2);
        fprintf(CPPFP,"protected:\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"_PBJ_Internal::");
        sendCppMessageLevels(ctx,CPPFP);
        fprintf(CPPFP,"%s *super;\n",id->chars);
        sendTabs(ctx,1);
        fprintf(CPPFP,"public:\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"%s() {\n",id->chars);
        sendTabs(ctx,3);
        fprintf(CPPFP,"super=this;\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"}\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"%s(_PBJ_Internal::",id->chars);
        sendCppMessageLevels(ctx,CPPFP);
        fprintf(CPPFP,"%s &reference) {\n",id->chars);
        sendTabs(ctx,3);
        fprintf(CPPFP,"super=&reference;\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"}\n",id->chars);

        sendTabs(ctx,2);
        fprintf(CPPFP,"%s(const _PBJ_Internal::",id->chars);
        sendCppMessageLevels(ctx,CPPFP);
        fprintf(CPPFP,"%s &copy) {\n",id->chars);
        sendTabs(ctx,3);
        fprintf(CPPFP,"super=this;\n");
        sendTabs(ctx,3);
        fprintf(CPPFP,"*super=copy;\n");
        sendTabs(ctx,2);
        fprintf(CPPFP,"}\n",id->chars);
        
    }
}
void defineExtension(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    if (CPPFP&&0) {
        sendTabs(ctx,1);
        fprintf(CPPFP,"class %sExtend : public %s {\n",id->chars,id->chars);
        sendTabs(ctx,1);
        fprintf(CPPFP,"public:\n",id->chars,id->chars);
    }
}
const char *getProtoType(pPBJParser ctx, pANTLR3_STRING type) {
    int *flagBits=NULL;
    if (flagBits=(int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,type->chars)) {
        if (*flagBits>64){
            fprintf(stderr, "Error bitflag %d too high on enum %s\n",*flagBits,type->chars);
        }
        if (*flagBits>32) {
            return "::google::protobuf::uint64";
        }
        if (*flagBits>16) {
            return "::google::protobuf::uint32";
        }
        if (*flagBits>8) {
            return "::google::protobuf::uint16";
        }
        return "::google::protobuf::uint8";
    }
    if (strcmp((char*)type->chars,"string")==0)
        return "::std::string";
    if (strcmp((char*)type->chars,"bytes")==0)
        return "::std::string";
    if (strcmp((char*)type->chars,"uuid")==0)
        return "::std::string";
    if (strcmp((char*)type->chars,"time")==0)
        return "::google::protobuf::int64";
    if (strcmp((char*)type->chars,"duration")==0)
        return "::google::protobuf::int64";
    if (strcmp((char*)type->chars,"angle")==0)
        return "float";

    if (strcmp((char*)type->chars,"sint64")==0)
        return "::google::protobuf::int64";
    if (strcmp((char*)type->chars,"int64")==0)
        return "::google::protobuf::int64";
    if (strcmp((char*)type->chars,"sint32")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"int32")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"sint16")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"int16")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"sint8")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"int8")==0)
        return "::google::protobuf::int32";


    if (strcmp((char*)type->chars,"sfixed64")==0)
        return "::google::protobuf::int64";
    if (strcmp((char*)type->chars,"sfixed32")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"sfixed16")==0)
        return "::google::protobuf::int32";
    if (strcmp((char*)type->chars,"sfixed8")==0)
        return "::google::protobuf::int32";

    if (strcmp((char*)type->chars,"fixed64")==0)
        return "::google::protobuf::uint64";
    if (strcmp((char*)type->chars,"uint64")==0)
        return "::google::protobuf::uint64";
    if (strcmp((char*)type->chars,"fixed32")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"uint32")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"fixed16")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"uint16")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"fixed8")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"uint8")==0)
        return "::google::protobuf::uint32";
    if (strcmp((char*)type->chars,"float")==0||strcmp((char*)type->chars,"normal")==0||strcmp((char*)type->chars,"vector2f")==0||strcmp((char*)type->chars,"vector3f")==0||strcmp((char*)type->chars,"vector4f")==0||strcmp((char*)type->chars,"quaternion")==0||strcmp((char*)type->chars,"boundingsphere3f")==0||strcmp((char*)type->chars,"boundingbox3f3f")==0)
        return "float";
    if (strcmp((char*)type->chars,"double")==0||strcmp((char*)type->chars,"vector2d")==0||strcmp((char*)type->chars,"vector3d")==0||strcmp((char*)type->chars,"vector4d")==0||strcmp((char*)type->chars,"boundingsphere3d")==0||strcmp((char*)type->chars,"boundingbox3d3f")==0)
        return "double";
    return (const char*)type->chars;
}
int getNumItemsPerElement(pPBJParser ctx, pANTLR3_STRING type) {
    if (strcmp((char*)type->chars,"normal")==0||strcmp((char*)type->chars,"vector2f")==0||strcmp((char*)type->chars,"vector2d")==0)
        return 2;
    if (strcmp((char*)type->chars,"quaternion")==0||strcmp((char*)type->chars,"vector3f")==0||strcmp((char*)type->chars,"vector3d")==0)
        return 3;
    if (strcmp((char*)type->chars,"vector4f")==0||strcmp((char*)type->chars,"vector4d")==0||strcmp((char*)type->chars,"boundingsphere3f")==0||strcmp((char*)type->chars,"boundingsphere3d")==0)
        return 4;
    if (strcmp((char*)type->chars,"boundingbox3f3f")==0||strcmp((char*)type->chars,"boundingbox3d3f")==0)
        return 6;
    return 1;
}
const char *getCppType(pPBJParser ctx, pANTLR3_STRING type) {
    int *flagBits=NULL;
    if (flagBits=(int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,type->chars)) {
        if (*flagBits>32) {
            return "PBJ::uint64";
        }
        if (*flagBits>16) {
            return "PBJ::uint32";
        }
        if (*flagBits>8) {
            return "PBJ::uint16";
        }
        return "PBJ::uint8";
    }
    if (strcmp((char*)type->chars,"string")==0)
        return "::std::string";
    if (strcmp((char*)type->chars,"bytes")==0)
        return "::std::string";
    if (strcmp((char*)type->chars,"uuid")==0)
        return "PBJ::UUID";
    if (strcmp((char*)type->chars,"time")==0)
        return "PBJ::Time";
    if (strcmp((char*)type->chars,"duration")==0)
        return "PBJ::Duration";
    if (strcmp((char*)type->chars,"angle")==0)
        return "float";
    if (strcmp((char*)type->chars,"sint64")==0)
        return "PBJ::int64";
    if (strcmp((char*)type->chars,"int64")==0)
        return "PBJ::int64";
    if (strcmp((char*)type->chars,"sint32")==0)
        return "PBJ::int32";
    if (strcmp((char*)type->chars,"int32")==0)
        return "PBJ::int32";
    if (strcmp((char*)type->chars,"sint16")==0)
        return "PBJ::int16";
    if (strcmp((char*)type->chars,"int16")==0)
        return "PBJ::int16";
    if (strcmp((char*)type->chars,"sint8")==0)
        return "PBJ::int8";
    if (strcmp((char*)type->chars,"int8")==0)
        return "PBJ::int8";

    if (strcmp((char*)type->chars,"sfixed64")==0)
        return "PBJ::int64";
    if (strcmp((char*)type->chars,"sfixed32")==0)
        return "PBJ::int32";
    if (strcmp((char*)type->chars,"sfixed16")==0)
        return "PBJ::int16";
    if (strcmp((char*)type->chars,"sfixed8")==0)
        return "PBJ::int8";

    if (strcmp((char*)type->chars,"fixed64")==0)
        return "PBJ::uint64";
    if (strcmp((char*)type->chars,"uint64")==0)
        return "PBJ::uint64";
    if (strcmp((char*)type->chars,"fixed32")==0)
        return "PBJ::uint32";
    if (strcmp((char*)type->chars,"uint32")==0)
        return "PBJ::uint32";
    if (strcmp((char*)type->chars,"fixed16")==0)
        return "PBJ::uint16";
    if (strcmp((char*)type->chars,"uint16")==0)
        return "PBJ::uint16";
    if (strcmp((char*)type->chars,"fixed8")==0)
        return "PBJ::uint8";
    if (strcmp((char*)type->chars,"uint8")==0)
        return "PBJ::uint8";
    if (strcmp((char*)type->chars,"normal")==0)
        return "PBJ::Vector3f";
    if (strcmp((char*)type->chars,"vector2f")==0)
        return "PBJ::Vector2f";        
    if (strcmp((char*)type->chars,"vector2d")==0)
        return "PBJ::Vector2f";        
    if (strcmp((char*)type->chars,"quaternion")==0)
        return "PBJ::Quaternion";
    if (strcmp((char*)type->chars,"vector3f")==0)
        return "PBJ::Vector3f";
    if (strcmp((char*)type->chars,"vector3d")==0)
        return "PBJ::Vector3d";
    if (strcmp((char*)type->chars,"vector4f")==0)
        return "PBJ::Vector4f";
    if (strcmp((char*)type->chars,"vector4d")==0)
        return "PBJ::Vector4d";
    if (strcmp((char*)type->chars,"boundingsphere3f")==0)
        return "PBJ::BoundingSphere3f";
    if (strcmp((char*)type->chars,"boundingsphere3d")==0)
        return "PBJ::BoundingSphere3d";
    if (strcmp((char*)type->chars,"boundingbox3f3f")==0)
        return "PBJ::BoundingBox3f3f";
    if (strcmp((char*)type->chars,"boundingbox3d3f")==0)
        return "PBJ::BoundingBox3d3f";
    return (char*)type->chars;
}
const char *getPBJType(pPBJParser ctx, pANTLR3_STRING type) {
    if (strcmp((char*)type->chars,"angle")==0) {
        return "PBJ::angle";
    }
    if (strcmp((char*)type->chars,"string")==0) {
        return "PBJ::utf8string";
    }
    if (strcmp((char*)type->chars,"bytes")==0) {
        return "PBJ::bytes";
    }
    if (strcmp((char*)type->chars,"normal")==0) {
        return "PBJ::normal";
    }
    return getCppType(ctx,type);

}
void printFlags(FILE *fp, pANTLR3_HASH_TABLE flag_all_on,pANTLR3_STRING name) {
    pANTLR3_STRING all_on =((pANTLR3_STRING)(flag_all_on->get(flag_all_on,name->chars)));
    if (all_on) {
        fprintf(fp,"%s",all_on->chars);
    }else {
        printf ("Invalid flags value %s\n",name->chars);
    }
}
void defineField(pPBJParser ctx, pANTLR3_STRING type, pANTLR3_STRING name, pANTLR3_STRING value, int notRepeated, int isMultiplicitiveAdvancedType){
    if (SCOPE_TOP(Symbols)->message==NULL) {
        fprintf(CPPFP,"using _PBJ_Internal::%s;\n",name->chars);
        return;
    }
    const char * cppType=getCppType(ctx,type);
    const char * pbjType=getPBJType(ctx,type);
    const char * protoType=getProtoType(ctx,type);
    int isEnum = SCOPE_TOP(Symbols)->enum_sizes->get(SCOPE_TOP(Symbols)->enum_sizes,type->chars)!=NULL;
    int isFlag = SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,type->chars)!=NULL;
    int isMessageType=((SCOPE_TOP(Symbols)->types->get(SCOPE_TOP(Symbols)->types,type->chars)!=NULL)&&!isEnum)&&!isFlag;
    int isRepeated=!notRepeated;
    if (CPPFP) {
        sendTabs(ctx,1);fprintf(CPPFP,"inline void clear_%s() {return super->clear_%s();}\n",name->chars,name->chars);
    }
    if (isMultiplicitiveAdvancedType) {
        int numItemsPerElement=getNumItemsPerElement(ctx,type);
        if (CPPFP) {
            int i;
            if (isRepeated) {
                sendTabs(ctx,1);fprintf(CPPFP,"inline int %s_size() const {return super->%s_size()/%d;}\n",name->chars,name->chars,numItemsPerElement);
                sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s(int index) const {assert(index<%s_size()&&index>=0);return true;}\n",name->chars,name->chars);
            }else {
                sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s() const {return super->%s_size()>=%d;}\n",name->chars,name->chars,numItemsPerElement);
            }
            sendTabs(ctx,1);fprintf(CPPFP,"inline %s %s(%s) const {\n",cppType,name->chars,isRepeated?"int index":"");
            sendTabs(ctx,2);fprintf(CPPFP,"if (has_%s(%s)) {\n",name->chars,isRepeated?"index":"");
            sendTabs(ctx,3);fprintf(CPPFP,"return _PBJCast< %s>()(",pbjType);
            for (i=0;i<numItemsPerElement;++i) {
                if (isRepeated) {
                    fprintf(CPPFP,"super->%s(index*%d+%d)%s",name->chars,numItemsPerElement,i,i+1==numItemsPerElement?");\n":",");
                }else {
                    fprintf(CPPFP,"super->%s(%d)%s",name->chars,i,i+1==numItemsPerElement?");\n":",");
                }
            }
            sendTabs(ctx,2);fprintf(CPPFP,"} else {\n");
            if (value) {

                sendTabs(ctx,3);fprintf(CPPFP,"return %s(%s);",cppType,value->chars);
            }else {
                sendTabs(ctx,3);fprintf(CPPFP,"return _PBJCast< %s>()();\n",pbjType);
            }
            sendTabs(ctx,2);fprintf(CPPFP,"}\n");
            sendTabs(ctx,1);fprintf(CPPFP,"}\n");
            sendTabs(ctx,1);fprintf(CPPFP,"inline void %s_%s(const %s &value) {\n",isRepeated?"add":"set",name->chars,cppType);
            if (!isRepeated){
                sendTabs(ctx,2);fprintf(CPPFP,"super->clear_%s();\n",name->chars);
            }
            sendTabs(ctx,2);fprintf(CPPFP,"_PBJConstruct< %s>::ArrayType _PBJtempArray=_PBJConstruct< %s>()(value);\n",pbjType,pbjType);
            for (i=0;i<numItemsPerElement;++i) {
                sendTabs(ctx,2);fprintf(CPPFP,"super->add_%s(_PBJtempArray[%d]);\n",name->chars,i);
            }
            sendTabs(ctx,1);fprintf(CPPFP,"}\n");

            if (isRepeated) {
                sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(int index, const %s &value) {\n",name->chars,cppType);
                for (i=0;i<numItemsPerElement;++i) {
                    sendTabs(ctx,2);fprintf(CPPFP,"super->set_%s(index*%d+%d,_PBJConstruct< %s>()(value)[%d]);\n",name->chars,numItemsPerElement,i,pbjType,i);
                }
                sendTabs(ctx,1);fprintf(CPPFP,"}\n");
            }
        }

    }else {
        //set or add
        if (isMessageType) {
            sendTabs(ctx,1);fprintf(CPPFP,"inline %s %s_%s() {\n",cppType,isRepeated?"add":"mutable",name->chars);
            sendTabs(ctx,2);fprintf(CPPFP,"return *super->%s_%s();\n",isRepeated?"add":"mutable",name->chars);
            sendTabs(ctx,1);fprintf(CPPFP,"}\n");
        }else {
            sendTabs(ctx,1);fprintf(CPPFP,"inline void %s_%s(const %s &value) const {\n",isRepeated?"add":"set",name->chars,cppType);
            if (isEnum) {
                sendTabs(ctx,2);fprintf(CPPFP,"super->%s_%s((_PBJ_Internal::",isRepeated?"add":"set",name->chars);
                sendCppMessageLevels(ctx,CPPFP);
                fprintf(CPPFP,"%s::%s)value);\n",SCOPE_TOP(Symbols)->message->chars,type->chars);
            }else {
                sendTabs(ctx,2);fprintf(CPPFP,"super->%s_%s(_PBJConstruct< %s>()(value));\n",isRepeated?"add":"set",name->chars,pbjType);
            }
            sendTabs(ctx,1);fprintf(CPPFP,"}\n");                
        }
        if (isRepeated) {
            if (CPPFP) {
                
                sendTabs(ctx,1);fprintf(CPPFP,"inline int %s_size() const {return super->%s_size();}\n",name->chars,name->chars);

                if (strcmp((char*)type->chars,"bytes")==0||strcmp((char*)type->chars,"string")==0) {//strings and bytes have special setter functionality
                    sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(int index, const char *value) const {\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"super->set_%s(index,value);\n",name->chars);
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    sendTabs(ctx,1);fprintf(CPPFP,"inline void add_%s(const char *value) const {\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"super->add_%s(value);\n",name->chars);
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    sendTabs(ctx,1);fprintf(CPPFP,"inline const ::std::string& %s(int index) const {\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"return super->%s(index);\n",name->chars);
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");                    
                    sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s(int index) const {assert(index>=0&&index<%s_size()); return true;}\n",name->chars,name->chars);
                    
                    if (strcmp((char*)type->chars,"bytes")==0) {
                        sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(int index, const void *value, size_t size) const {\n",name->chars);
                        sendTabs(ctx,2);fprintf(CPPFP,"super->set_%s(index,value,size);\n",name->chars);
                        sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                        sendTabs(ctx,1);fprintf(CPPFP,"inline void add_%s(const void *value, size_t size) const {\n",name->chars);
                        sendTabs(ctx,2);fprintf(CPPFP,"super->add_%s(value,size);\n",name->chars);
                        sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    }
                }else {
                    if (isMessageType) {
                        sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s(int index) const {assert(index>=0&&index<%s_size()); return true;}\n",name->chars,name->chars,name->chars);
                    }else if (isFlag) {
                        int i,numFlags=0;
                        sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s(int index) const {",name->chars);
                        sendTabs(ctx,2);fprintf(CPPFP,"assert(index>=0&&index<%s_size();\n",name->chars); 
                        sendTabs(ctx,2);fprintf(CPPFP,"return _PBJValidateFlags< %s>()(super->%s(index),",pbjType,name->chars);
                        
                        printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type);
                        fprintf(CPPFP,");\n");
                        sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    }else {
                        sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s(int index) const {assert(index>=0&&index<%s_size()); return _PBJValidate<%s>()(super->%s(index));}\n",name->chars,name->chars,pbjType,name->chars);
                    }
                    sendTabs(ctx,1);fprintf(CPPFP,"inline %s %s(int index) const {\n",cppType,name->chars);
                    if (value) {
                        sendTabs(ctx,2);fprintf(CPPFP,"if (has_%s(index)) {\n",name->chars);
                    }
                    if (isMessageType) {
                        sendTabs(ctx,value?3:2);fprintf(CPPFP,"return %s(super->%s(index));\n",cppType,name->chars);//FIXME:cast
                    } else if (isFlag) {
                        sendTabs(ctx,value?3:2);fprintf(CPPFP,"return _PBJCastFlags< %s>(index)(super->%s(index),",pbjType,name->chars);
                        printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type);
                        fprintf(CPPFP,");\n",cppType,name->chars);
                    } else {
                        sendTabs(ctx,value?3:2);fprintf(CPPFP,"return (%s)_PBJCast< %s>()(super->%s(index));\n",cppType,pbjType,name->chars);
                    }
                    if (value) {
                        sendTabs(ctx,2);fprintf(CPPFP,"} else {\n");
                        if(value) {
                            sendTabs(ctx,3);fprintf(CPPFP,"return %s(%s);\n",cppType,value->chars);
                        }else {
                            sendTabs(ctx,3);fprintf(CPPFP,"%s _retval; return _retval;\n",cppType);
                        }
                        sendTabs(ctx,2);fprintf(CPPFP,"}\n");
                    }
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                }
                if (isMessageType) {
                    sendTabs(ctx,1);fprintf(CPPFP,"inline %s set_%s(int index) {\n",cppType,name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"return *super->mutable_%s(index);\n",name->chars);
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                }else {
                    sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(int index, const %s &value) const {\n",name->chars,cppType);
                    if (isEnum) {
                        sendTabs(ctx,2);fprintf(CPPFP,"return super->set_%s(index,(_PBJ_Internal::",name->chars);
                        sendCppMessageLevels(ctx,CPPFP);
                        fprintf(CPPFP,"%s::%s)(value));\n",SCOPE_TOP(Symbols)->message->chars,type->chars);
                    }else {
                        sendTabs(ctx,2);fprintf(CPPFP,"return super->set_%s(index,_PBJConstruct< %s>()(value));\n",name->chars,pbjType);
                    }
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");                
                }
                
            }
            
        }else {
            if (CPPFP){
                if (isMessageType) {
                    sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s() const {return super->has_%s();}\n",name->chars,name->chars);
                }else if (isFlag) {
                    sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s() const {\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"if (!super->has_%s()) return false;\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"return _PBJValidateFlags< %s>()(super->%s(),",pbjType,name->chars);
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type);                    
                    fprintf(CPPFP,");\n");
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                }else {
                    sendTabs(ctx,1);fprintf(CPPFP,"inline bool has_%s() const {return super->has_%s()&&_PBJValidate<%s>()(super->%s());}\n",name->chars,name->chars,pbjType,name->chars);
                }
                sendTabs(ctx,1);fprintf(CPPFP,"inline %s %s() const {\n",cppType,name->chars);
                if (value) {
                    sendTabs(ctx,2);fprintf(CPPFP,"if (has_%s()) {\n",name->chars);
                }
                if (isMessageType) {
                    sendTabs(ctx,value?3:2);fprintf(CPPFP,"return %s(super->%s());\n",cppType,name->chars);
                } else if (isFlag) {
                    sendTabs(ctx,value?3:2);fprintf(CPPFP,"return _PBJCastFlags< %s>()(super->%s(),",pbjType,name->chars);
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type);
                    fprintf(CPPFP,");\n",cppType,name->chars);
                } else {
                    sendTabs(ctx,value?3:2);fprintf(CPPFP,"return (%s)_PBJCast< %s>()(super->%s());\n",cppType,pbjType,name->chars);
                }
                if (value) {
                    sendTabs(ctx,2);fprintf(CPPFP,"} else {\n");
                    if(value) {
                        sendTabs(ctx,3);fprintf(CPPFP,"return %s(%s);\n",cppType,value->chars);
                    }else {
                        sendTabs(ctx,3);fprintf(CPPFP,"%s _retval; return _retval;\n",cppType);
                    }
                    sendTabs(ctx,2);fprintf(CPPFP,"}\n");
                }
                sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                if (strcmp((char*)type->chars,"bytes")==0||strcmp((char*)type->chars,"string")==0) {//strings and bytes have special setter functionality
                    sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(const char *value) const {\n",name->chars);
                    sendTabs(ctx,2);fprintf(CPPFP,"super->set_%s(value);\n",name->chars);
                    sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    
                    if (strcmp((char*)type->chars,"bytes")==0) {
                        sendTabs(ctx,1);fprintf(CPPFP,"inline void set_%s(const void *value, size_t size) const {\n",name->chars);
                        sendTabs(ctx,2);fprintf(CPPFP,"super->set_%s(value,size);\n",name->chars);
                        sendTabs(ctx,1);fprintf(CPPFP,"}\n");
                    }
                }
            }
        }
    }
}
void printEnum(pPBJParser ctx, int offset, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    int enumSize=enumValues->size(enumValues);
    int i;
    if (CPPFP){
        sendTabs(ctx,1);
        
        fprintf(CPPFP,"enum %s {\n",id->chars);
        for (i=0;i<enumSize;i+=2) {
            pANTLR3_STRING enumVal=((pANTLR3_STRING)(enumValues->get(enumValues,i)));
            sendTabs(ctx,2);
            fprintf(CPPFP,"%s=_PBJ_Internal::%s::%s%s",
                    enumVal->chars,
                    SCOPE_TOP(Symbols)->message->chars,
                    enumVal->chars,
                    (i+2==enumSize?"\n":",\n"));            
        }
        sendTabs(ctx,1);
        fprintf(CPPFP,"};\n");
    }
}
void defineEnum(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    int i,*maxval=(int*)malloc(sizeof(int));
    *maxval=0;
    if (SCOPE_TOP(Symbols) == NULL) return;
    defineType(ctx,id);
    if (CPPFP) {
        printEnum(ctx,1,id,enumValues);
    }
    
    ANTLR3_INT32 size=enumValues->size(enumValues);
    for (i=0;i<size;++i) {
        void * elem=enumValues->get(enumValues,i);
        int val=atoi((char*)((pANTLR3_STRING)elem)->chars);
        if (val>*maxval) *maxval=val;
    }
    SCOPE_TOP(Symbols)->enum_sizes->put(SCOPE_TOP(Symbols)->enum_sizes,id->chars,maxval,&free);        
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
    SCOPE_TOP(Symbols)->flag_sizes->put(SCOPE_TOP(Symbols)->flag_sizes,id->chars,bits,&free);
    {
        int i;
        ANTLR3_INT32 size=flagValues->size(flagValues);
        pANTLR3_STRING allFlagsOn=id->factory->newRaw(id->factory);
        allFlagsOn->addc(allFlagsOn,'0');
        for (i=0;i<size;++i) {
            void * elem=flagValues->get(flagValues,i);
            pANTLR3_STRING str=(pANTLR3_STRING)elem;
            if (str->chars[0]>='0'&&str->chars[0]<='9') {

            }else {
                allFlagsOn->addc(allFlagsOn,'|');
                allFlagsOn->appendS(allFlagsOn,str);
            }
        }
        SCOPE_TOP(Symbols)->flag_all_on->put(SCOPE_TOP(Symbols)->flag_all_on,id->chars,allFlagsOn,stringFree);
    }
}

void defineFlagValue(pPBJParser ctx, pANTLR3_STRING messageName, pANTLR3_STRING flagName, pANTLR3_LIST flagValues, pANTLR3_STRING id, pANTLR3_STRING value) {
    if (SCOPE_TOP(Symbols) == NULL) return;//FIXME
    SCOPE_TOP(Symbols)->flag_values->put(SCOPE_TOP(Symbols)->flag_values, id->chars, id, NULL);
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
    if (CPPFP&&0) {
        sendTabs(ctx,0);
        fprintf(CPPFP,"};\n");
    }
    closeNamespace(ctx);
}
    
void  NameSpaceFree(pPBJParser_NameSpace_SCOPE scope)
{
    ANTLR3_FREE(scope);
}

SCOPE_TYPE(NameSpace)
NameSpacePush(pPBJParser ctx)
{
    /* Pointer used to create a new set of attributes
     */
    pPBJParser_NameSpace_SCOPE      newAttributes;

    /* Allocate the memory for a new structure if we need one.
     */
    if (ctx->pPBJParser_NameSpaceStack->size(ctx->pPBJParser_NameSpaceStack) > ctx->pPBJParser_NameSpaceStack_limit)
    {
        // The current limit value was less than the number of scopes available on the stack so
        // we can just reuse one. Our limit tracks the stack count, so the index of the entry we want
        // is one less than that, or conveniently, the current value of limit.
        //
        newAttributes = (pPBJParser_NameSpace_SCOPE)ctx->pPBJParser_NameSpaceStack->get(ctx->pPBJParser_NameSpaceStack, ctx->pPBJParser_NameSpaceStack_limit);
    }
    else
    {
        // Need a new allocation
        //
        newAttributes = (pPBJParser_NameSpace_SCOPE) ANTLR3_MALLOC(sizeof(PBJParser_NameSpace_SCOPE));
        if  (newAttributes != NULL)
        {
            /* Standard ANTLR3 library implementation
             */
            ctx->pPBJParser_NameSpaceStack->push(ctx->pPBJParser_NameSpaceStack, newAttributes, (void (*)(void *))NameSpaceFree);
        }
    }

    // Blank out any previous free pointer, the user might or might install a new one.
    //
    newAttributes->free = NULL;

    // Indicate the position in the available stack that the current level is at
    //
    ctx->pPBJParser_NameSpaceStack_limit++;

	/* Return value is the pointer to the new entry, which may be used locally
	 * without de-referencing via the context.
     */
    return  newAttributes;
}
void
NameSpacePop(pPBJParser ctx)
{
    // First see if the user defined a function they want to be called when a
    // scope is popped/freed.
    //
	// If the user supplied the scope entries with a free function,then call it first
	//
    if	(SCOPE_TOP(NameSpace)->free != NULL)
	{
        SCOPE_TOP(NameSpace)->free(SCOPE_TOP(NameSpace));
	}

    // Now we decrement the scope's upper limit bound. We do not actually pop the scope as
    // we want to reuse scope entries if we do continuous push and pops. Most scopes don't
    // next too far so we don't want to keep freeing and allocating them
    //
    ctx->pPBJParser_NameSpaceStack_limit--;
    SCOPE_TOP(NameSpace) = (pPBJParser_NameSpace_SCOPE)(ctx->pPBJParser_NameSpaceStack->get(ctx->pPBJParser_NameSpaceStack, ctx->pPBJParser_NameSpaceStack_limit - 1));
}
