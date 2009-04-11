#include <antlr3.h>
#include "PBJ.h"
#include "PBJParseUtil.h"
#include <assert.h>
#include <sstream>
#include <iostream>
#include "PBJLanguageOutput.hpp"
void  freeSymbolTable(SCOPE_TYPE(Symbols) symtab) {
    symtab->types->free(symtab->types);
    symtab->flag_sizes->free(symtab->flag_sizes);
    symtab->enum_sizes->free(symtab->enum_sizes);
    symtab->flag_values->free(symtab->flag_values);
    symtab->flag_all_on->free(symtab->flag_all_on);
    symtab->enum_values->free(symtab->enum_values);
    symtab->required_advanced_fields->free(symtab->required_advanced_fields);
    if (symtab->message) {
        stringFree(symtab->message);
    }
    delete symtab->cs_streams->csType;
    delete symtab->cs_streams->csBuild;
    delete symtab->cs_streams->csMembers;
    free(symtab->cs_streams);
    symtab->cs_streams=NULL;
       
}
void  freeNameSpace(SCOPE_TYPE(NameSpace) symtab) {
    symtab->imports->free(symtab->imports);
    //delete symtab->output->cpp;    
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
    if (symtab->output->cpp||symtab->output->cs) {
        char lst;
        if (symtab->filename->len>6) {
            lst=symtab->filename->chars[symtab->filename->len-6];
            assert(lst=='.');
            symtab->filename->chars[symtab->filename->len-6]='\0';
        }
        if (symtab->output->cpp) {
            *symtab->output->cpp<<"#include \"pbj.hpp\"\n";
            *symtab->output->cpp<<"#include \""<<symtab->filename->chars<<".pb.h\"\n";
        }
        if (symtab->filename->len>6) {
            symtab->filename->chars[symtab->filename->len-6]=lst;
        }
        if (symtab->output->cs) {
            *symtab->output->cs<<"using converters = global::PBJ.Converters\n";
            *symtab->output->cs<<"using pbd = global::Google.ProtocolBuffers.Descriptors;\n";
        }
    }
    symtab->package=NULL;
    symtab->imports = antlr3ListNew(1);
    symtab->free=freeNameSpace;
    
}
void  initSymbolTable(SCOPE_TYPE(Symbols) symtab, pANTLR3_STRING messageName, int isExtension) {
    symtab->cs_streams=(struct CsStreams*)malloc(sizeof(struct CsStreams));
    memset(symtab->cs_streams,0,sizeof(struct CsStreams));
    symtab->cs_streams->csType=new std::stringstream;
    symtab->cs_streams->csBuild=new std::stringstream;
    symtab->cs_streams->csMembers=new std::stringstream;

    symtab->message=NULL;
    symtab->required_advanced_fields = antlr3ListNew(1);
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

#define CPPFP *SCOPE_TOP(NameSpace)->output->cpp 
#define CSFP *SCOPE_TOP(NameSpace)->output->cs
#define CSTYPE *SCOPE_TOP(Symbols)->cs_streams->csType
#define CSBUILD *SCOPE_TOP(Symbols)->cs_streams->csBuild
#define CSMEM *SCOPE_TOP(Symbols)->cs_streams->csMembers
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
        CPPFP<<"#include \""<<s->chars<<".pbj.hpp\"\n";
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
        if (CSFP)
            CSFP<<"namespace ";

        do {
            if (CPPFP)
                CPPFP<<"namespace "<<substr->chars<<" {\n";
            if (CSFP)
                CSFP<<"namespace "<<substr->chars;

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
            if (substr&&CSFP) {
                CSFP<<'.';
            }else {
                CSFP<<" {\n";
            }
        }while (substr);
    }
}
static void closeNamespace(pPBJParser ctx) {
    if (SCOPE_TOP(NameSpace)->package&&SCOPE_SIZE(Symbols)<=2) {
        size_t stringSize=SCOPE_TOP(NameSpace)->package->size;
        size_t i;
        if (CSFP&&stringSize) {
            CSFP<<"}\n";
        }
        if (CPPFP&&stringSize) {
            CPPFP<<"}\n";
            for (i=0;i<stringSize;++i) {       
                if (SCOPE_TOP(NameSpace)->package->chars[i]=='.') {
                    CPPFP<<"}\n";
                }
            }
        }
    }
}
pANTLR3_STRING defaultValuePreprocess(pPBJParser ctx, pANTLR3_STRING type, pANTLR3_STRING value){
    return stringDup(value);
}

static std::ostream& sendTabs(pPBJParser ctx,int offset) {
    int num=SCOPE_SIZE(Symbols)+offset-1;
    int i;
    for (i=0;i<num;++i) {
        CPPFP<<"    ";
    }
    return CPPFP;
}

static std::ostream& sendTabs(pPBJParser ctx,std::ostream &os,int offset) {
    int num=SCOPE_SIZE(Symbols)+offset-1;
    int i;
    for (i=0;i<num;++i) {
        os<<"    ";
    }
    return os;
}

static std::ostream& sendCppNs(pPBJParser ctx, std::ostream&fp,const char*delim="::") {
    int i;
    for (i=0;i+1<SCOPE_SIZE(Symbols);++i) {
        if (((SCOPE_TYPE(Symbols))(SCOPE_INSTANCE(Symbols,i)))->message) {
            fp<<delim<<((SCOPE_TYPE(Symbols))SCOPE_INSTANCE(Symbols,i))->message->chars;
        }
    }
    return fp;
}
static std::ostream& sendHashNs(pPBJParser ctx, std::ostream&fp) {
    int i;
    unsigned int delim=0;
    for (i=0;i+1<SCOPE_SIZE(Symbols);++i) {
        if (((SCOPE_TYPE(Symbols))(SCOPE_INSTANCE(Symbols,i)))->message) {
            pANTLR3_STRING msg=((SCOPE_TYPE(Symbols))SCOPE_INSTANCE(Symbols,i))->message;
            int i;
            for (i=0;i<msg->len;++i){ 
                delim=delim^(msg->chars[i]+i*63);
            }
            fp<<msg->chars<<delim;
        }
    }
    return fp;
}
static std::ostream& sendCsNs(pPBJParser ctx,  std::ostream&fp,const char*delim=".") {
    return sendCppNs(ctx,fp,delim);
}
void defineMessage(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    SCOPE_TOP(Symbols)->message=stringDup(id);
    if (CPPFP) {
/*
        sendTabs(ctx,1)<<"class "<<id->chars;
        sendHashNs(ctx,CPPFP)<<"HasFields : public PBJ::HasFields {\n";
        sendTabs(ctx,2)<<"template <class Message> bool operator() (const Message* thus) const {\n";
        sendTabs(ctx,3)<<"return evaluateInput(computeHasFields(thus));\n";
        sendTabs(ctx,2)<<"}\n";
        sendTabs(ctx,2)<<"template <class Message> bool operator() (const Message* thus, bool isOutput) const {\n";
        sendTabs(ctx,3)<<"return evaluateOutput(computeHasFields(thus));\n";
        sendTabs(ctx,2)<<"}\n";
        sendTabs(ctx,2)<<"template <class Message> bool computeHasFields(const Message* thus) const;\n";
        sendTabs(ctx,1)<<"};\n";
*/
        sendTabs(ctx,1)<<"class "<<id->chars<<" : public PBJ::Message< "<<id->chars<<" > {\n";
        sendTabs(ctx,1)<<"protected:\n";
        sendTabs(ctx,2)<<"_PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<" superconstructed;\n";

        sendTabs(ctx,2)<<"_PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<" *super;\n";
        sendTabs(ctx,1)<<"public:\n";
        sendTabs(ctx,2)<<"_PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<"* _PBJSuper(){ return super; }\n";

        sendTabs(ctx,2)<<id->chars<<"():PBJ::Message< "<<id->chars<<"> (&superconstructed) {\n";
        sendTabs(ctx,3)<<"super=&superconstructed;\n";
        sendTabs(ctx,2)<<"}\n";
        sendTabs(ctx,2)<<id->chars<<"(_PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<" &reference):PBJ::Message< "<<id->chars<<" >(&reference) {\n";
        sendTabs(ctx,3)<<"super=&reference;\n";
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<id->chars<<"(const _PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<" &copy):PBJ::Message< "<<id->chars<<" >(&superconstructed), superconstructed(copy) {\n";
        sendTabs(ctx,3)<<"super=&superconstructed;\n";
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<id->chars<<"& operator=(const "<<id->chars<<" &copy) {\n";
        sendTabs(ctx,3)<<"setMessageRepresentation(&superconstructed);\n";
        sendTabs(ctx,3)<<"super=&superconstructed;\n";
        sendTabs(ctx,3)<<"*super=*copy.super;\n";
        sendTabs(ctx,3)<<"return *this;\n";
        sendTabs(ctx,2)<<"}\n";


        sendTabs(ctx,2)<<"inline static "<<id->chars<<" default_instance() {\n";
        sendTabs(ctx,3)<<id->chars<<" _internalStaticVar(_PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<"::default_instance());\n";
        sendTabs(ctx,3)<<"return _internalStaticVar;\n";        
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<"static const ::google::protobuf::Descriptor* descriptor(){\n";
        sendTabs(ctx,3)<<"return _PBJ_Internal";
        sendCppNs(ctx,CPPFP)<<"::"<<id->chars<<"::descriptor();";
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<"inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const{\n";
        sendTabs(ctx,3)<<"return super->unknown_fields();\n";
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<"inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields(){\n";
        sendTabs(ctx,3)<<"return super->mutable_unknown_fields();\n";
        sendTabs(ctx,2)<<"}\n";
        

        sendTabs(ctx,2)<<"const ::google::protobuf::Descriptor* GetDescriptor() const {\n";
        sendTabs(ctx,3)<<"return super->GetDescriptor();\n";
        sendTabs(ctx,2)<<"}\n";

        sendTabs(ctx,2)<<"const ::google::protobuf::Reflection* GetReflection() const {\n";
        sendTabs(ctx,3)<<"return super->GetReflection();\n";
        sendTabs(ctx,2)<<"}\n";
        sendTabs(ctx,2)<<"int GetCachedSize()const{ return super->GetCachedSize(); }\n";
        sendTabs(ctx,2)<<id->chars<<"* New()const{ return new "<<id->chars<<"; }\n";
    }
    if (CSFP) {
        sendTabs(ctx,CSFP,1)<<"class "<<id->chars<<" : public PBJ.Message {\n";
        sendTabs(ctx,CSFP,2)<<"protected _PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"."<<id->chars<<" super;\n";
        sendTabs(ctx,CSFP,2)<<"public _PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"."<<id->chars<<" _PBJSuper{ get { return super;} }\n";

        sendTabs(ctx,CSFP,2)<<"public "<<id->chars<<"() {\n";
        sendTabs(ctx,CSFP,3)<<"super=new _PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"."<<id->chars<<"();\n";
        sendTabs(ctx,CSFP,2)<<"}\n";
        sendTabs(ctx,CSFP,2)<<"public "<<id->chars<<"(_PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"::"<<id->chars<<" reference) {\n";
        sendTabs(ctx,CSFP,3)<<"super=reference;\n";
        sendTabs(ctx,CSFP,2)<<"}\n";

        sendTabs(ctx,CSFP,2)<<"public static "<<id->chars<<" defaultInstance= new "<<id->chars<<" (_PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"."<<id->chars<<".DefaultInstance;\n";
        sendTabs(ctx,CSFP,2)<<"public static "<<id->chars<<" DefaultInstance{\n";
        sendTabs(ctx,CSFP,3)<<"get {return defaultInstance;}\n";
        sendTabs(ctx,CSFP,2)<<"}\n";

        sendTabs(ctx,CSFP,2)<<"public static pbd.MessageDescriptor Descriptor {\n";
        sendTabs(ctx,CSFP,3)<<"get { return _PBJ_Internal";
        sendCsNs(ctx,CSFP)<<"."<<id->chars<<".Descriptor; }";
        sendTabs(ctx,CSFP,2)<<"}\n";    
        sendTabs(ctx,CSFP,2)<< "public static class Types {\n";
        
    }
}
void defineExtension(pPBJParser ctx, pANTLR3_STRING id){
    openNamespace(ctx);
    if (CPPFP&&0) {
        sendTabs(ctx,1)<<"class "<<id->chars<<"Extend : public "<<id->chars<<" {\n";
        sendTabs(ctx,1)<<"public:\n";
    }
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
const char *getCsType(pPBJParser ctx, pANTLR3_STRING type) {
    int *flagBits=NULL;
    if (flagBits=(int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,type->chars)) {
        if (*flagBits>32) {
            return "PBJ.uint64";
        }
        if (*flagBits>16) {
            return "PBJ.uint32";
        }
        if (*flagBits>8) {
            return "PBJ.uint16";
        }
        return "PBJ.uint8";
    }
    if (strcmp((char*)type->chars,"string")==0)
        return "string";
    if (strcmp((char*)type->chars,"bytes")==0)
        return "PBJ.pb::ByteString";
    if (strcmp((char*)type->chars,"uuid")==0)
        return "PBJ.UUID";
    if (strcmp((char*)type->chars,"time")==0)
        return "PBJ.Time";
    if (strcmp((char*)type->chars,"duration")==0)
        return "PBJ.Duration";
    if (strcmp((char*)type->chars,"angle")==0)
        return "float";
    if (strcmp((char*)type->chars,"sint64")==0)
        return "PBJ.int64";
    if (strcmp((char*)type->chars,"int64")==0)
        return "PBJ.int64";
    if (strcmp((char*)type->chars,"sint32")==0)
        return "PBJ.int32";
    if (strcmp((char*)type->chars,"int32")==0)
        return "PBJ.int32";
    if (strcmp((char*)type->chars,"sint16")==0)
        return "PBJ.int16";
    if (strcmp((char*)type->chars,"int16")==0)
        return "PBJ.int16";
    if (strcmp((char*)type->chars,"sint8")==0)
        return "PBJ.int8";
    if (strcmp((char*)type->chars,"int8")==0)
        return "PBJ.int8";

    if (strcmp((char*)type->chars,"sfixed64")==0)
        return "PBJ.int64";
    if (strcmp((char*)type->chars,"sfixed32")==0)
        return "PBJ.int32";
    if (strcmp((char*)type->chars,"sfixed16")==0)
        return "PBJ.int16";
    if (strcmp((char*)type->chars,"sfixed8")==0)
        return "PBJ.int8";

    if (strcmp((char*)type->chars,"fixed64")==0)
        return "PBJ.uint64";
    if (strcmp((char*)type->chars,"uint64")==0)
        return "PBJ.uint64";
    if (strcmp((char*)type->chars,"fixed32")==0)
        return "PBJ.uint32";
    if (strcmp((char*)type->chars,"uint32")==0)
        return "PBJ.uint32";
    if (strcmp((char*)type->chars,"fixed16")==0)
        return "PBJ.uint16";
    if (strcmp((char*)type->chars,"uint16")==0)
        return "PBJ.uint16";
    if (strcmp((char*)type->chars,"fixed8")==0)
        return "PBJ.uint8";
    if (strcmp((char*)type->chars,"uint8")==0)
        return "PBJ.uint8";
    if (strcmp((char*)type->chars,"normal")==0)
        return "PBJ.Vector3f";
    if (strcmp((char*)type->chars,"vector2f")==0)
        return "PBJ.Vector2f";        
    if (strcmp((char*)type->chars,"vector2d")==0)
        return "PBJ.Vector2f";        
    if (strcmp((char*)type->chars,"quaternion")==0)
        return "PBJ.Quaternion";
    if (strcmp((char*)type->chars,"vector3f")==0)
        return "PBJ.Vector3f";
    if (strcmp((char*)type->chars,"vector3d")==0)
        return "PBJ.Vector3d";
    if (strcmp((char*)type->chars,"vector4f")==0)
        return "PBJ.Vector4f";
    if (strcmp((char*)type->chars,"vector4d")==0)
        return "PBJ.Vector4d";
    if (strcmp((char*)type->chars,"boundingsphere3f")==0)
        return "PBJ.BoundingSphere3f";
    if (strcmp((char*)type->chars,"boundingsphere3d")==0)
        return "PBJ.BoundingSphere3d";
    if (strcmp((char*)type->chars,"boundingbox3f3f")==0)
        return "PBJ.BoundingBox3f3f";
    if (strcmp((char*)type->chars,"boundingbox3d3f")==0)
        return "PBJ.BoundingBox3d3f";
    return (char*)type->chars;
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
        return "PBJ::Vector2d";        
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
const char *getArrayType(pPBJParser ctx, pANTLR3_STRING type) {
    if (strcmp((char*)type->chars,"normal")==0)
        return "float";
    if (strcmp((char*)type->chars,"vector2f")==0)
        return "float";        
    if (strcmp((char*)type->chars,"vector2d")==0)
        return "double";        
    if (strcmp((char*)type->chars,"quaternion")==0)
        return "float";
    if (strcmp((char*)type->chars,"vector3f")==0)
        return "float";
    if (strcmp((char*)type->chars,"vector3d")==0)
        return "double";
    if (strcmp((char*)type->chars,"vector4f")==0)
        return "float";
    if (strcmp((char*)type->chars,"vector4d")==0)
        return "double";
    if (strcmp((char*)type->chars,"boundingsphere3f")==0)
        return "float";
    if (strcmp((char*)type->chars,"boundingsphere3d")==0)
        return "double";
    if (strcmp((char*)type->chars,"boundingbox3f3f")==0)
        return "float";
    if (strcmp((char*)type->chars,"boundingbox3d3f")==0)
        return "double";
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

const char *getPBJCsType(pPBJParser ctx, pANTLR3_STRING type) {
    if (strcmp((char*)type->chars,"angle")==0) {
        return "PBJ.angle";
    }
    if (strcmp((char*)type->chars,"string")==0) {
        return "PBJ.utf8string";
    }
    if (strcmp((char*)type->chars,"bytes")==0) {
        return "PBJ.bytes";
    }
    if (strcmp((char*)type->chars,"normal")==0) {
        return "PBJ.normal";
    }
    return getCppType(ctx,type);

}
std::ostream& printFlags(std::ostream&fp, pANTLR3_HASH_TABLE flag_all_on,pANTLR3_STRING name) {
    pANTLR3_STRING all_on =((pANTLR3_STRING)(flag_all_on->get(flag_all_on,name->chars)));
    if (all_on) {
        fp.write((char*)all_on->chars,all_on->len);
    }else {
        fprintf (stderr,"Invalid flags value %s\n",name->chars);
    }
    return fp;
}
pANTLR3_STRING toFirstUpper(pANTLR3_STRING name) {
    pANTLR3_STRING uname=stringDup(name);
    uname->chars[0]=toupper(name->chars[0]);
    return uname;
}
void defineField(pPBJParser ctx, pANTLR3_STRING type, pANTLR3_STRING name, pANTLR3_STRING value, int notRepeated, int isRequired, int isMultiplicitiveAdvancedType){
    if (isMultiplicitiveAdvancedType&&isRequired) {
        SCOPE_TOP(Symbols)->required_advanced_fields->put(SCOPE_TOP(Symbols)->required_advanced_fields,SCOPE_TOP(Symbols)->required_advanced_fields->size(SCOPE_TOP(Symbols)->required_advanced_fields),stringDup(name),&stringFree);
    }
    if (SCOPE_TOP(Symbols)->message==NULL) {
        if (CPPFP) {
            CPPFP<<"using _PBJ_Internal::"<<name->chars<<";\n";
        }
        if (CSFP) {
            CSFP<<"using "<<name->chars<<" = _PBJ_Internal."<<name->chars<<";\n";
        }
        return;
    }
    pANTLR3_STRING uname=toFirstUpper(name);
    pANTLR3_STRING utype=toFirstUpper(type);
    const char * cppType=getCppType(ctx,type);
    const char * csType=getCsType(ctx,type);
    const char * pbjType=getPBJType(ctx,type);
    int isEnum = SCOPE_TOP(Symbols)->enum_sizes->get(SCOPE_TOP(Symbols)->enum_sizes,type->chars)!=NULL;
    int isFlag = SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,type->chars)!=NULL;
    int isMessageType=((SCOPE_TOP(Symbols)->types->get(SCOPE_TOP(Symbols)->types,type->chars)!=NULL)&&!isEnum)&&!isFlag;
    int isRepeated=!notRepeated;
    std::stringstream csShared;
    if (CPPFP) {
        sendTabs(ctx,1)<<"inline void clear_"<<name->chars<<"() {return super->clear_"<<name->chars<<"();}\n";
    }
    if (CSFP) {
        sendTabs(ctx,CSBUILD,1)<<"inline void Clear"<<uname->chars<<"() { return super->Clear"<<uname->chars<<"();} }\n";
    }
    if (isMultiplicitiveAdvancedType) {
        int numItemsPerElement=getNumItemsPerElement(ctx,type);
        if (CPPFP) {
            int i;
            if (isRepeated) {
                sendTabs(ctx,csShared,1)<<"inline int "<<uname->chars<<"Count { get { return super->"<<uname->chars<<"Count/"<<numItemsPerElement<<";} }\n";
                sendTabs(ctx,1)<<"inline int "<<name->chars<<"_size() const {return super->"<<name->chars<<"_size()/"<<numItemsPerElement<<";}\n";

                sendTabs(ctx,csShared,1)<<"inline bool Has"<<uname->chars<<"(int index) { return true; }\n";
                sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"(int index) const {assert(index<"<<name->chars<<"_size()&&index>=0);return true;}\n";
            }else {
                sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"() const {return super->"<<name->chars<<"_size()>="<<numItemsPerElement<<";}\n";
                sendTabs(ctx,csShared,1)<<"inline bool Has"<<uname->chars<<"{ get {return super->"<<uname->chars<<"Count>="<<numItemsPerElement<<";} }\n";
            }
            sendTabs(ctx,csShared,1)<<csType<<" "<<uname->chars<<(isRepeated?"(int index)":"{ get ")<<" {\n";
            sendTabs(ctx,csShared,2)<<"if (Has"<<uname->chars<<(isRepeated?"(index)":"")<<") {\n";
            sendTabs(ctx,csShared,3)<<"return PBJ._PBJCast"<<utype->chars<<"(";
            sendTabs(ctx,1)<<"inline "<<cppType<<" "<<name->chars<<"("<<(isRepeated?"int index":"")<<") const {\n";
            sendTabs(ctx,2)<<"if (has_"<<name->chars<<"("<<(isRepeated?"index":"")<<")) {\n";
            sendTabs(ctx,3)<<"return _PBJCast< "<<pbjType<<">()(";
            for (i=0;i<numItemsPerElement;++i) {
                if (isRepeated) {
                    csShared<<"super->"<<uname->chars<<"(index*"<<numItemsPerElement<<"+"<<i<<")"<<(i+1==numItemsPerElement?");\n":",");
                    CPPFP<<"super->"<<name->chars<<"(index*"<<numItemsPerElement<<"+"<<i<<")"<<(i+1==numItemsPerElement?");\n":",");
                }else {
                    csShared<<"super->"<<uname->chars<<"(index*"<<numItemsPerElement<<"+"<<i<<")"<<(i+1==numItemsPerElement?");\n":",");
                    CPPFP<<"super->"<<name->chars<<"("<<i<<")"<<(i+1==numItemsPerElement?");\n":",");
                }
            }
            sendTabs(ctx,csShared,2)<<"} else {\n";            
            sendTabs(ctx,2)<<"} else {\n";
            if (value) {
                sendTabs(ctx,csShared,3)<<"return new "<<csType<<"("<<value->chars<<");";
                sendTabs(ctx,3)<<"return "<<cppType<<"("<<value->chars<<");";
            }else {
                sendTabs(ctx,csShared,3)<<"return PBJ._PBJCast"<<utype->chars<<"();\n";
                sendTabs(ctx,3)<<"return _PBJCast< "<<pbjType<<">()();\n";
            }
            sendTabs(ctx,2)<<"}\n";
            sendTabs(ctx,1)<<"}\n";
            sendTabs(ctx,csShared,2)<<"}\n";
            sendTabs(ctx,csShared,1)<<"}\n";
            {
                std::string temp=csShared.str();
                CSBUILD <<temp;
                CSMEM <<temp;
            }
            if (isRepeated==false) {//need a getter which needs an xtra brace  and then a setter
                sendTabs(ctx,CSBUILD,1)<<"set {\n";
            }else {
                sendTabs(ctx,CSBUILD,1)<<"Builder Add"<<uname->chars<<"("<<csType<<" value) {\n";
            }

            sendTabs(ctx,1)<<"inline void "<<(isRepeated?"add":"set")<<"_"<<name->chars<<"(const "<<cppType<<" &value) {\n";
            if (!isRepeated){
                sendTabs(ctx,2)<<"super->clear_"<<name->chars<<"();\n";
                sendTabs(ctx,CSBUILD,2)<<"super->Clear"<<uname->chars<<"();\n";
            }
            sendTabs(ctx,2)<<"_PBJConstruct< "<<pbjType<<">::ArrayType _PBJtempArray=_PBJConstruct< "<<pbjType<<">()(value);\n";
            sendTabs(ctx,CSBUILD,2)<<getArrayType(ctx,type)<<"[] _PBJtempArray=_PBJConstruct"<<utype->chars<<"(value);\n";
            for (i=0;i<numItemsPerElement;++i) {
                sendTabs(ctx,2)<<"super->add_"<<name->chars<<"(_PBJtempArray["<<i<<"]);\n";
                sendTabs(ctx,CSBUILD,2)<<"super->Add"<<uname->chars<<"(_PBJtempArray["<<i<<"]);\n";
            }
            sendTabs(ctx,CSBUILD,1)<<"}\n";
            sendTabs(ctx,1)<<"}\n";
            if (isRepeated==false) {
                sendTabs(ctx,CSBUILD,1)<<"}\n";
                sendTabs(ctx,CSMEM,1)<<"}\n";
            }
            if (isRepeated) {
                sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(int index, const "<<cppType<<" &value) {\n";
                sendTabs(ctx,2)<<"_PBJConstruct< "<<pbjType<<">::ArrayType _PBJtempArray=_PBJConstruct< "<<pbjType<<">()(value);\n";
                sendTabs(ctx,CSBUILD,1)<<"Builder Set"<<uname->chars<<"(index,"<<csType<<" value) {\n";
                sendTabs(ctx,CSBUILD,2)<<getArrayType(ctx,type)<<"[] _PBJtempArray=_PBJConstruct"<<utype->chars<<"(value);\n";
                for (i=0;i<numItemsPerElement;++i) {
                    sendTabs(ctx,2)<<"super->set_"<<name->chars<<"(index*"<<numItemsPerElement<<"+"<<i<<",_PBJtempArray["<<i<<"]);\n";
                    sendTabs(ctx,CSBUILD,2)<<"super->Set"<<uname->chars<<"(index*"<<numItemsPerElement<<"+"<<i<<",_PBJtempArray["<<i<<"]);\n";
                }
                sendTabs(ctx,CSBUILD,1)<<"}\n";
                sendTabs(ctx,1)<<"}\n";
            }
        }
    }else {
        if (isRepeated) {
            if (CPPFP) {
                
                sendTabs(ctx,1)<<"inline int "<<name->chars<<"_size() const {return super->"<<name->chars<<"_size();}\n";
                sendTabs(ctx,csShared,1)<<"public int "<<uname->chars<<"Count { get { return super."<<uname->chars<<"Count;} }\n";
                bool isRawByteArray=(strcmp((char*)type->chars,"bytes")==0||strcmp((char*)type->chars,"string")==0);
                if (isRawByteArray) {//strings and bytes have special setter functionality
                    sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(int index, const char *value) const {\n";
                    sendTabs(ctx,2)<<"super->set_"<<name->chars<<"(index,value);\n";
                    sendTabs(ctx,1)<<"}\n";
                    sendTabs(ctx,1)<<"inline void add_"<<name->chars<<"(const char *value) const {\n";
                    sendTabs(ctx,2)<<"super->add_"<<name->chars<<"(value);\n";
                    sendTabs(ctx,1)<<"}\n";
/*
                    sendTabs(ctx,1)<<"inline const ::std::string& "<<name->chars<<"(int index) const {\n";
                    sendTabs(ctx,2)<<"return super->"<<name->chars<<"(index);\n";
                    sendTabs(ctx,1)<<"}\n";                    
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"(int index) const {assert(index>=0&&index<"<<name->chars<<"_size()); return true;}\n";
*/
                    if (strcmp((char*)type->chars,"bytes")==0) {
                        sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(int index, const void *value, size_t size) const {\n";
                        sendTabs(ctx,2)<<"super->set_"<<name->chars<<"(index,value,size);\n";
                        sendTabs(ctx,1)<<"}\n";
                        sendTabs(ctx,1)<<"inline void add_"<<name->chars<<"(const void *value, size_t size) const {\n";
                        sendTabs(ctx,2)<<"super->add_"<<name->chars<<"(value,size);\n";
                        sendTabs(ctx,1)<<"}\n";
                    }
                }
                if (isMessageType||isEnum) {
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"(int index) const {assert(index>=0&&index<"<<name->chars<<"_size()); return true;}\n";
                    sendTabs(ctx,csShared,1)<<"inline bool Has"<<uname->chars<<"(int index) const {return true;}\n";
                }else if (isFlag) {
                    int i,numFlags=0;
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"(int index) const {";
                    sendTabs(ctx,2)<<"assert(index>=0&&index<"<<name->chars<<"_size();\n";
                    sendTabs(ctx,2)<<"return _PBJValidateFlags< "<<pbjType<<">()(super->"<<name->chars<<"(index),";
                                        
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                    sendTabs(ctx,1)<<"}\n";


                    sendTabs(ctx,csShared,1)<<"inline bool Has"<<uname->chars<<"(int index) const {";
                    sendTabs(ctx,csShared,2)<<"return PBJ._PBJValidateFlags(super."<<uname->chars<<"(index),";
                                        
                    printFlags(csShared,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                    sendTabs(ctx,csShared,1)<<"}\n";

                }else {
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"(int index) const {assert(index>=0&&index<"<<name->chars<<"_size()); return _PBJValidate< "<<pbjType<<">()(super->"<<name->chars<<"(index));}\n";
                    sendTabs(ctx,csShared,1)<<"inline bool Has"<<uname->chars<<"(int index) {return PBJ._PBJValidate"<<utype->chars<<"(super."<<uname->chars<<"(index));}\n";
                }
                sendTabs(ctx,csShared,1)<<"public "<<csType<<" "<<uname->chars<<"(int index) {\n";
                sendTabs(ctx,1)<<"inline "<<(isRawByteArray?"const std::string&":cppType)<<" "<<name->chars<<"(int index) const {\n";
                if (value) {
                    sendTabs(ctx,2)<<"if (has_"<<name->chars<<"(index)) {\n";
                    sendTabs(ctx,csShared,2)<<"if (Has"<<uname->chars<<"(index)) {\n";
                }
                if (isMessageType) {
                    sendTabs(ctx,value?3:2)<<"return "<<cppType<<"(super->"<<name->chars<<"(index));\n";//FIXME:cast
                    sendTabs(ctx,csShared,value?3:2)<<"return "<<type->chars<<"(super."<<uname->chars<<"(index));\n";//FIXME:cast
                } else if (isFlag) {
                    sendTabs(ctx,value?3:2)<<"return _PBJCastFlags< "<<pbjType<<">()(super->"<<name->chars<<"(index),";
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";

                    sendTabs(ctx,csShared,value?3:2)<<"return PBJ._PBJCastFlags"<<utype->chars<<"(super."<<name->chars<<"(index),";
                    printFlags(csShared,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                } else if (isEnum) {
                    sendTabs(ctx,value?3:2)<<"return ("<<type->chars<<")(super->"<<name->chars<<"(index))\n;";
                    sendTabs(ctx,csShared,value?3:2)<<"return ("<<type->chars<<")(super."<<uname->chars<<"(index))\n;";

                } else {
                    sendTabs(ctx,value?3:2)<<"return _PBJCast< "<<pbjType<<">()(super->"<<name->chars<<"(index));\n";
                    sendTabs(ctx,csShared,value?3:2)<<"return ("<<csType<<")_PBJCast"<<utype->chars<<"(super->"<<name->chars<<"(index));\n";
                }
                if (value) {
                    sendTabs(ctx,2)<<"} else {\n";
                    sendTabs(ctx,csShared,2)<<"} else {\n";
                    if(value) {
                        sendTabs(ctx,3)<<"return "<<cppType<<"("<<value->chars<<");\n";
                        sendTabs(ctx,csShared,3)<<"return new "<<csType<<"("<<value->chars<<");\n";
                    }else {
                        sendTabs(ctx,value?3:2)<<"return ("<<cppType<<")_PBJCast"<<(isMessageType?"Message":"")<<"< "<<pbjType<<">()();\n";
                        sendTabs(ctx,csShared,value?3:2)<<"return _PBJCast"<<utype->chars<<"();\n";
                    }
                    sendTabs(ctx,2)<<"}\n";
                    sendTabs(ctx,csShared,2)<<"}\n";
                }
                sendTabs(ctx,1)<<"}\n";
                sendTabs(ctx,csShared,1)<<"}\n";
            }
            if (isMessageType) {
                sendTabs(ctx,CSBUILD,1)<<"public Builder Set"<<uname->chars<<"(int index,"<<type->chars<<" value) {\n";
                sendTabs(ctx,CSBUILD,2)<<"super.Set"<<uname->chars<<"(index,value._PBJSuper);\n";
                sendTabs(ctx,CSBUILD,2)<<"return this;\n";
                sendTabs(ctx,CSBUILD,1)<<"}\n";
                
                sendTabs(ctx,1)<<"inline "<<cppType<<" set_"<<name->chars<<"(int index) {\n";
                sendTabs(ctx,2)<<"return *super->mutable_"<<name->chars<<"(index);\n";
                sendTabs(ctx,1)<<"}\n";
            }else {
                sendTabs(ctx,CSBUILD,1)<<"public Builder Set"<<uname->chars<<"(int index, "<<csType<<" value) {\n";
                
                sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(int index, const "<<cppType<<" &value) const {\n";
                if (isEnum) {
                    sendTabs(ctx,CSBUILD,2)<<"return super->set_"<<name->chars<<"(index,(_PBJ_Internal";
                    sendCsNs(ctx,CSBUILD)<<"."<<SCOPE_TOP(Symbols)->message->chars<<".Types."<<type->chars<<")(value));\n";
   
                    sendTabs(ctx,2)<<"return super->set_"<<name->chars<<"(index,(_PBJ_Internal";
                    sendCppNs(ctx,CPPFP)<<"::"<<SCOPE_TOP(Symbols)->message->chars<<"::"<<type->chars<<")(value));\n";
                }else {
                    if (isFlag) {
                        sendTabs(ctx,CSBUILD,2)<<"super->Set"<<uname->chars<<"(index,value);\n";
                    }else {
                        sendTabs(ctx,CSBUILD,2)<<"super->Set"<<uname->chars<<"(index,_PBJConstruct"<<type->chars<<"(value));\n";
                    }
                    sendTabs(ctx,CSBUILD,2)<<"return this;\n";
                    sendTabs(ctx,2)<<"return super->set_"<<name->chars<<"(index,_PBJConstruct< "<<pbjType<<">()(value));\n";
                }
                sendTabs(ctx,CSBUILD,1)<<"}\n";
                sendTabs(ctx,1)<<"}\n";
            }
        }else {
            if (CPPFP){
                if (strcmp((char*)type->chars,"bytes")==0||strcmp((char*)type->chars,"string")==0) {//strings and bytes have special setter functionality
                    sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(const char *value) const {\n";
                    sendTabs(ctx,2)<<"super->set_"<<name->chars<<"(value);\n";
                    sendTabs(ctx,1)<<"}\n";
                    
                    if (strcmp((char*)type->chars,"bytes")==0) {
                        sendTabs(ctx,1)<<"inline void set_"<<name->chars<<"(const void *value, size_t size) const {\n";
                        sendTabs(ctx,2)<<"super->set_"<<name->chars<<"(value,size);\n";
                        sendTabs(ctx,1)<<"}\n";
                    }
                }
                if (isMessageType||isEnum) {
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"() const {return super->has_"<<name->chars<<"();}\n";
                    sendTabs(ctx,csShared,1)<<"public bool Has"<<uname->chars<<"get { {return super->Has"<<uname->chars<<"();} }\n";
                }else if (isFlag) {
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"() const {\n";
                    sendTabs(ctx,2)<<"if (!super->has_"<<name->chars<<"()) return false;\n";
                    sendTabs(ctx,2)<<"return _PBJValidateFlags< "<<pbjType<<">()(super->"<<name->chars<<"(),";
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                    sendTabs(ctx,1)<<"}\n";

                    sendTabs(ctx,csShared,1)<<"public bool Has"<<uname->chars<<" { get {\n";
                    sendTabs(ctx,csShared,2)<<"if (!super->Has"<<uname->chars<<") return false;\n";
                    sendTabs(ctx,csShared,2)<<"return PBJ._PBJValidateFlags"<<type->chars<<"()(super."<<name->chars<<",";
                    printFlags(csShared,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                    sendTabs(ctx,csShared,1)<<"} }\n";


                }else {
                    sendTabs(ctx,csShared,1)<<"public bool Has"<<uname->chars<<"{ get {return super->Has"<<uname->chars<<"&&PBJ._PBJValidate"<<utype->chars<<"(super."<<uname->chars<<");} }\n";
                    sendTabs(ctx,1)<<"inline bool has_"<<name->chars<<"() const {return super->has_"<<name->chars<<"()&&_PBJValidate<"<<pbjType<<">()(super->"<<name->chars<<"());}\n";
                }


                sendTabs(ctx,1)<<"inline "<<cppType<<" "<<name->chars<<"() const {\n";
                sendTabs(ctx,2)<<"if (has_"<<name->chars<<"()) {\n";
                sendTabs(ctx,csShared,1)<<"public "<<csType<<" "<<uname->chars<<"{ get {\n";
                sendTabs(ctx,csShared,2)<<"if (Has"<<uname->chars<<"()) {\n";
                if (isMessageType) {
                    sendTabs(ctx,3)<<"return "<<type->chars<<"(super->"<<name->chars<<"());\n";
                    sendTabs(ctx,csShared,3)<<"return new "<<type->chars<<"(super."<<name->chars<<"());\n";
                } else if (isFlag) {
                    sendTabs(ctx,3)<<"return _PBJCastFlags< "<<pbjType<<">()(super->"<<name->chars<<"(),";
                    printFlags(CPPFP,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                    
                    sendTabs(ctx,csShared,3)<<"return PBJ._PBJCastFlags(super."<<name->chars<<"(),";
                    printFlags(csShared,SCOPE_TOP(Symbols)->flag_all_on,type)<<");\n";
                } else {
                    sendTabs(ctx,3)<<"return ("<<cppType<<")_PBJCast< "<<pbjType<<">()(super->"<<name->chars<<"());\n";
                    sendTabs(ctx,csShared,3)<<"return PBJ._PBJCast"<<type->chars<<"(super."<<name->chars<<"());\n";
                }
                sendTabs(ctx,2)<<"} else {\n";
                if(value) {
                    sendTabs(ctx,3)<<"return "<<cppType<<"("<<value->chars<<");\n";
                    sendTabs(ctx,csShared,3)<<"return new "<<csType<<"("<<value->chars<<");\n";
                }else {
                    sendTabs(ctx,csShared,3)<<"return _PBJCast"<<type->chars<<"();\n";
                    sendTabs(ctx,3)<<"return _PBJCast"<<(isMessageType?"Message":"")<<" < "<<pbjType<<"> ()();\n";
                }
                sendTabs(ctx,2)<<"}\n";
                sendTabs(ctx,csShared,2)<<"}\n";
                sendTabs(ctx,1)<<"}\n";
                sendTabs(ctx,csShared,1)<<"}\n";

            }
        }
        {
            std::string temp=csShared.str();
            CSBUILD<<temp;
            CSMEM<<temp;
        }
        //set or add
        if (isMessageType) {
            if (isRepeated) {
                sendTabs(ctx,CSBUILD,1)<<csType<<"Add"<<uname->chars<<"() {\n";
                sendTabs(ctx,CSBUILD,1)<<"return super->"<<"Add"<<uname->chars<<"();\n";
                sendTabs(ctx,CSBUILD,1)<<"}\n";
            }else {                                                                                                     
                sendTabs(ctx,CSBUILD,1)<<"set {";
                sendTabs(ctx,CSBUILD,1)<<"return super->"<<uname->chars<<";\n";
                sendTabs(ctx,CSBUILD,1)<<"}\n";
            }
            sendTabs(ctx,1)<<"inline "<<cppType<<" "<<(isRepeated?"add":"mutable")<<"_"<<name->chars<<"() {\n";
            sendTabs(ctx,2)<<"return *super->"<<(isRepeated?"add":"mutable")<<"_"<<name->chars<<"();\n";
            sendTabs(ctx,1)<<"}\n";
        }else {
            if (isRepeated) {
                sendTabs(ctx,CSBUILD,1)<<csType<<"Add"<<uname->chars<<"() {\n";
            }else {
                sendTabs(ctx,CSBUILD,1)<<"set {";
            }
            sendTabs(ctx,1)<<"inline void "<<(isRepeated?"add":"set")<<"_"<<name->chars<<"(const "<<cppType<<" &value) const {\n";
            if (isEnum) {
                sendTabs(ctx,2)<<"super->"<<(isRepeated?"add":"set")<<"_"<<name->chars<<"((_PBJ_Internal";
                sendCppNs(ctx,CPPFP)<<"::"<<SCOPE_TOP(Symbols)->message->chars<<"::"<<type->chars<<")value);\n";
                sendTabs(ctx,CSBUILD,2)<<"super."<<(isRepeated?"Add":"")<<name->chars<<(isRepeated?"":"=")<<"((_PBJ_Internal";
                sendCsNs(ctx,CSBUILD)<<"."<<SCOPE_TOP(Symbols)->message->chars<<"::"<<type->chars<<")value);\n";

            }else {
                sendTabs(ctx,CSBUILD,2)<<"super."<<(isRepeated?"Add":"")<<name->chars<<(isRepeated?"":"=")<<(isFlag?"((":"(PBJ._PBJ_Construct(")<<"value));\n";
                sendTabs(ctx,2)<<"super->"<<(isRepeated?"add":"set")<<"_"<<name->chars<<"(_PBJConstruct< "<<pbjType<<">()(value));\n";
            }
            sendTabs(ctx,1)<<"}\n";
            sendTabs(ctx,CSBUILD,1)<<"}\n";
        }
        if (!isRepeated) {
            sendTabs(ctx,CSBUILD,1)<<"}\n";//closing the !repeated set{ and get{
            sendTabs(ctx,CSMEM,1)<<"}\n";
        }
    }
    stringFree(uname);
    stringFree(utype);
}
void printEnum(pPBJParser ctx, int offset, pANTLR3_STRING id, pANTLR3_LIST enumValues) {
    int enumSize=enumValues->size(enumValues);
    int i;
    if (CPPFP){
        sendTabs(ctx,1)<<"enum "<<id->chars<<" {\n";
        sendTabs(ctx,CSFP,1)<<"enum "<<id->chars<<" {\n";
        for (i=0;i<enumSize;i+=2) {
            pANTLR3_STRING enumVal=((pANTLR3_STRING)(enumValues->get(enumValues,i)));
            sendTabs(ctx,2)<<enumVal->chars<<"=_PBJ_Internal::"<<SCOPE_TOP(Symbols)->message->chars<<"::"<<enumVal->chars<<(i+2==enumSize?"\n":",\n");            
            sendTabs(ctx,CSFP,2)<<enumVal->chars<<"=_PBJ_Internal."<<SCOPE_TOP(Symbols)->message->chars<<".Types."<<enumVal->chars<<(i+2==enumSize?"\n":",\n");            
        }
        sendTabs(ctx,1)<<"};\n";
        sendTabs(ctx,CSFP,1)<<"};\n";
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
    pANTLR3_LIST reqAdv=SCOPE_TOP(Symbols)->required_advanced_fields;
    if (CPPFP) {
        sendTabs(ctx,1)<<"bool _HasAllPBJFields() const {\n";//types
        sendTabs(ctx,2)<<"return true\n";
        {
            int i;
            int size=reqAdv->size(reqAdv);
            for (i=0;i<size;++i) {
                pANTLR3_STRING s=(pANTLR3_STRING)reqAdv->get(reqAdv,i);
                sendTabs(ctx,3)<<"&&has_"<<s->chars<<"()\n";
            }
            sendTabs(ctx,3)<<";\n";            
        }
        sendTabs(ctx,1)<<"}";
        
        sendTabs(ctx,0)<<"};\n";
    }
    if (CSFP) {
        int i;
        CSFP<<SCOPE_TOP(Symbols)->cs_streams->csType->str();
        sendTabs(ctx,CSFP,1)<<"}\n";//types
        sendTabs(ctx,CSFP,1)<<"protected bool _HasAllPBJFields() {\n";//types
        sendTabs(ctx,CSFP,2)<<"return true\n";
        {
            int i;
            int size=reqAdv->size(reqAdv);
            for (i=0;i<size;++i) {
                pANTLR3_STRING s=toFirstUpper((pANTLR3_STRING)reqAdv->get(reqAdv,i));
                sendTabs(ctx,CSFP,3)<<"&&Has"<<s<<"\n";
                stringFree(s);
            }
            sendTabs(ctx,CSFP,3)<<";\n";            
        }
        sendTabs(ctx,CSFP,1)<<"}";
        CSFP<<SCOPE_TOP(Symbols)->cs_streams->csMembers->str();
        sendTabs(ctx,CSFP,1)<<"class Builder {\n";//types
        CSFP<<SCOPE_TOP(Symbols)->cs_streams->csBuild->str();
        sendTabs(ctx,CSFP,1)<<"}\n";
        sendTabs(ctx,CSFP,0)<<"}\n";
    }
    closeNamespace(ctx);
}

void defineExtensionEnd(pPBJParser ctx, pANTLR3_STRING id){
    if (CPPFP&&0) {
        sendTabs(ctx,0)<<"};\n";
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
