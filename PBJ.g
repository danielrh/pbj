grammar PBJ;

options
{
    output = AST;
    language = C;
    ASTLabelType = pANTLR3_BASE_TREE;
}

tokens
{
    PROTO;
}

scope NameSpace {
    pANTLR3_STRING package;
    pANTLR3_LIST imports;
}

scope Symbols {
    pANTLR3_HASH_TABLE types;
    pANTLR3_HASH_TABLE flag_sizes;
    pANTLR3_HASH_TABLE enum_values;
}

@members {
    #include "PBJParseUtil.h"
}
protocol
    scope Symbols;
    @init {
        initSymbolTable(SCOPE_TOP(Symbols));
    }
    : protoroot ->protoroot
    ;

protoroot
    scope NameSpace;
    @init {
        initNameSpace(SCOPE_TOP(NameSpace));
    }
	:	importrule* (package importrule*)? message*
	;

package
   :   ( PACKAGELITERAL QUALIFIEDIDENTIFIER ITEM_TERMINATOR -> PACKAGELITERAL WS[" "] QUALIFIEDIDENTIFIER ITEM_TERMINATOR WS["\n"])
        {
            initPackage( SCOPE_TOP(NameSpace), $QUALIFIEDIDENTIFIER.text );
        }
	;

importrule
   :   ( IMPORTLITERAL STRING_LITERAL ITEM_TERMINATOR -> IMPORTLITERAL WS[" "] STRING_LITERAL ITEM_TERMINATOR WS["\n"] )
        {
            initImport( SCOPE_TOP(NameSpace), $STRING_LITERAL.text );
        }
	;


message
    scope {
        int isExtension;
        pANTLR3_STRING messageName;
    }
    :   ( message_or_extend message_identifier BLOCK_OPEN message_elements BLOCK_CLOSE -> message_or_extend WS[" "] message_identifier WS[" "] BLOCK_OPEN WS["\n"] message_elements BLOCK_CLOSE WS["\n"] )
        {
            if(!$message::isExtension) {
                defineType( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName );
            }
            stringFree($message::messageName);
        }
	;

message_or_extend : 
        MESSAGE {$message::isExtension=0;}
        |
        EXTEND {$message::isExtension=1;}
        ;

message_identifier
    : IDENTIFIER
    {
        $message::messageName=stringDup($IDENTIFIER.text);
    }
    ;

message_elements
    scope Symbols;
    @init
    {
        initSymbolTable(SCOPE_TOP(Symbols));  
    }
	:	message_element*
    {

    }
    ;

message_element
	:	field
	|	message
	|	enum_def
	|	flags_def
    |   extensions
	;

extensions : EXTENSIONS integer TO integer_inclusive ITEM_TERMINATOR -> WS["\t"] EXTENSIONS WS[" "] integer WS[" "] TO WS[" "] integer_inclusive ITEM_TERMINATOR WS["\n"] ;

integer_inclusive : integer ;

enum_def
    scope {
        pANTLR3_STRING enumName;
        pANTLR3_LIST enumList;
    }
    @init {
        $enum_def::enumList=antlr3ListNew(1);
    }
	:	( ENUM enum_identifier BLOCK_OPEN enum_element+ BLOCK_CLOSE -> WS["\t"] ENUM WS[" "] enum_identifier WS[" "] BLOCK_OPEN WS["\n"] (WS["\t"] enum_element)+ WS["\t"] BLOCK_CLOSE WS["\n"] )
        {
            defineEnum( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName, $enum_def::enumName, $enum_def::enumList);
            $enum_def::enumList->free($enum_def::enumList);
            stringFree($enum_def::enumName);
        }
	;

enum_element
	:	(IDENTIFIER EQUALS integer ITEM_TERMINATOR -> WS["\t"] IDENTIFIER WS[" "] EQUALS WS[" "] integer ITEM_TERMINATOR WS["\n"] )
        {
            defineEnumValue( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName, $enum_def::enumName, $enum_def::enumList, $IDENTIFIER.text, $integer.text );
        }
	;
enum_identifier
    : IDENTIFIER
      {
            $enum_def::enumName=stringDup($IDENTIFIER.text);
      }
      ;

flags_def
    scope
    {
        pANTLR3_STRING flagName;
        pANTLR3_LIST flagList;
        int flagBits;
    }
    @init {
        $flags_def::flagList=antlr3ListNew(1);
        
    }
	:	( flags flag_identifier BLOCK_OPEN flag_element+ BLOCK_CLOSE -> WS["\t"] ENUM["enum"] WS[" "] flag_identifier WS[" "] BLOCK_OPEN WS["\n"] (WS["\t"] flag_element)+ WS["\t"] BLOCK_CLOSE WS["\n"] )
        {
            defineFlag( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName, $flags_def::flagName, $flags_def::flagList, $flags_def::flagBits);
            $flags_def::flagList->free($flags_def::flagList);
            stringFree($flags_def::flagName);
        }
	;

flag_identifier 
	:	IDENTIFIER
        {
            $flags_def::flagName=stringDup($IDENTIFIER.text);
        }
	;

flag_element
	:	( IDENTIFIER EQUALS integer ITEM_TERMINATOR -> WS["\t"] IDENTIFIER WS[" "] EQUALS WS[" "] integer ITEM_TERMINATOR WS["\n"])
        {
            defineFlagValue( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName, $flags_def::flagName, $flags_def::flagList, $IDENTIFIER.text , $integer.text);
        }
	;

field
    scope{
        pANTLR3_STRING fieldType;
        pANTLR3_STRING fieldName;
        int fieldOffset;
    }
    :  (( ( (OPTIONAL multiplicitive_type field_name EQUALS field_offset default_value? ITEM_TERMINATOR ) | ( (REQUIRED|REPEATED) multiplicitive_type field_name EQUALS field_offset ITEM_TERMINATOR ) ) -> WS["\t"] REPEATED["repeated"] WS[" "] multiplicitive_type WS[" "] field_name WS[" "] EQUALS WS[" "] field_offset ITEM_TERMINATOR WS["\n"] )
     |
            ( (OPTIONAL field_type field_name EQUALS field_offset default_value? ITEM_TERMINATOR )  -> WS["\t"] OPTIONAL WS[" "] field_type WS[" "] field_name WS[" "] EQUALS WS[" "] field_offset WS[" "] default_value ITEM_TERMINATOR WS["\n"] )
     | 
            ( ( (REQUIRED|REPEATED) field_type field_name EQUALS field_offset ITEM_TERMINATOR ) -> WS["\t"] REQUIRED REPEATED WS[" "] field_type WS[" "] field_name WS[" "] EQUALS WS[" "] field_offset ITEM_TERMINATOR WS["\n"] ) )
    {
        stringFree($field::fieldName);
        stringFree($field::fieldType);
    }
	;

field_offset
    : integer
    {
        
        $field::fieldOffset=atoi((char*)($integer.text->chars));
    }
    ;

field_name
    : IDENTIFIER
    {
        $field::fieldName=stringDup($IDENTIFIER.text);
    }
    ;

field_type
    : type
    {
        $field::fieldType=stringDup($type.text);
    }
    | advanced_type
    {
       $field::fieldType=stringDup($advanced_type.text);
    }
    | ( IDENTIFIER 
        -> {SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)!=NULL
            && *(unsigned int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)<28}?
              FIXED32["uint32"] 
        -> {SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)!=NULL
            && *(unsigned int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)<=32}?
              FIXED32["fixed32"] 
        -> {SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)!=NULL
            && *(unsigned int*)SCOPE_TOP(Symbols)->flag_sizes->get(SCOPE_TOP(Symbols)->flag_sizes,$IDENTIFIER.text->chars)==64}?
             FIXED64["fixed64"] 
        -> IDENTIFIER )
    {
       $field::fieldType=stringDup($IDENTIFIER.text);
    }
    ;
multiplicitive_type
    : 
    multiplicitive_advanced_type 
    {
       $field::fieldType=stringDup($multiplicitive_advanced_type.text);        
    }
    ;

array_spec
	:	SQBRACKET_OPEN integer? SQBRACKET_CLOSE
	;

default_value
	:	SQBRACKET_OPEN DEFAULT EQUALS literal_value SQBRACKET_CLOSE
	;

type:		UINT32
	|	INT32
	|	SINT32
	|	FIXED32
	|	SFIXED32
	|	UINT64
	|	INT64
	|	SINT64
	|	FIXED64
	|	SFIXED64
	|	FLOAT
	|	DOUBLE
	|	STRING
	|	BYTES
	|	BOOL

	;
multiplicitive_advanced_type:
    |   NORMAL -> FLOAT["float"]
    |   VECTOR2F -> FLOAT["float"]
    |   VECTOR2D -> DOUBLE["double"]
    |   VECTOR3F -> FLOAT["float"]
    |   VECTOR3D -> DOUBLE["double"]
    |   VECTOR4F -> FLOAT["float"]
    |   VECTOR4D -> DOUBLE["double"]
    |   QUATERNION -> FLOAT["float"]
    |   BOUNDINGSPHERE3F -> FLOAT["float"]
    |   BOUNDINGSPHERE3D -> DOUBLE["double"]
    |   BOUNDINGBOX3F3F -> FLOAT["float"]
    |   BOUNDINGBOX3D3F -> DOUBLE["double"]
    ;

advanced_type:	UINT8 -> UINT32["uint32"]
	|	INT8 -> INT32["int32"]
	|	SINT8 -> SINT32["sint32"]
	|	FIXED8 -> INT32["uint32"]
	|	SFIXED8 -> INT32["sint32"]
	|	INT16 -> INT32["int32"]
	|	SINT16 -> SINT32["sint32"]
	|	FIXED16 -> INT32["uint32"]
	|	SFIXED16 -> INT32["sint32"]
    |   UINT16 -> UINT32["uint32"]
    |   UUID -> BYTES["bytes"]
    |   ANGLE -> FLOAT["float"]
    |   TIME -> FIXED64["fixed64"]
    |   DURATION -> SFIXED64["sfixed64"]
    ; 

literal_value
	:	HEX_LITERAL
    |   DECIMAL_LITERAL
    |   OCTAL_LITERAL
    |   FLOATING_POINT_LITERAL
    |   BOOL_LITERAL
    |   STRING_LITERAL
    ;

PACKAGELITERAL :    'package';
IMPORTLITERAL :     'import';

DOT :  '.';

// Message elements
MESSAGE	:	'message';
EXTEND	:	'extend';
EXTENSIONS : 'extensions';
TO : 'to';
// Enum elements
ENUM	:	'enum';

flags : 
     FLAGS8
     {
        $flags_def::flagBits=8;
     }
     |
     FLAGS16
     {
        $flags_def::flagBits=16;
     }
     |
     FLAGS32
     {
        $flags_def::flagBits=32;
     }
     |
     FLAGS64
     {
        $flags_def::flagBits=64;
     }

     ;
// Flags elements
FLAGS8	:	'flags8';
FLAGS16	:	'flags16';
FLAGS32	:	'flags32';
FLAGS64	:	'flags64';

// Field elements
REQUIRED:	'required';
OPTIONAL:	'optional';
REPEATED:	'repeated';

DEFAULT	:	'default';


EQUALS	:	'=';

// Common block elements
BLOCK_OPEN	:	'{';
BLOCK_CLOSE	:	'}';

ITEM_TERMINATOR
	:	';';

// Type elements
UINT8	:	'uint8';
INT8	:	'int8';
SINT8	:	'sint8';
FIXED8	:	'fixed8';
SFIXED8	:	'sfixed8';
UINT16	:	'uint16';
INT16	:	'int16';
SINT16	:	'sint16';
FIXED16	:	'fixed16';
SFIXED16:	'sfixed16';
UINT32	:	'uint32';
INT32	:	'int32';
SINT32	:	'sint32';
FIXED32	:	'fixed32';
SFIXED32:	'sfixed32';
UINT64	:	'uint64';
INT64	:	'int64';
SINT64	:	'sint64';
FIXED64	:	'fixed64';
SFIXED64:	'sfixed64';
FLOAT	:	'float';
DOUBLE	:	'double';
BOOL	:	'bool';
BYTES   :   'bytes';
STRING   :   'string';

// Advanced Type Elements
BYTE : 'byte';
UUID : 'uuid';
NORMAL : 'normal';
VECTOR2F : 'vector2f';
VECTOR2D : 'vector2d';
VECTOR3F : 'vector3f';
VECTOR3D : 'vector3d';
VECTOR4F : 'vector4f';
VECTOR4D : 'vector4d';
QUATERNION : 'quaternion';
ANGLE : 'angle';
TIME : 'time';
DURATION : 'duration';
BOUNDINGSPHERE3F : 'boundingsphere3f';
BOUNDINGSPHERE3D : 'boundingsphere3d';
BOUNDINGBOX3F3F : 'boundingbox3f3f';
BOUNDINGBOX3D3F : 'boundingbox3d3f';


SQBRACKET_OPEN	:	'[';
SQBRACKET_CLOSE	:	']';

integer
    : DECIMAL_LITERAL
    | HEX_LITERAL
    | OCTAL_LITERAL
    ;

STRING_LITERAL
    :  '"' STRING_GUTS '"'
    ;

fragment
STRING_GUTS :	( EscapeSequence | ~('\\'|'"') )* ;

BOOL_LITERAL
    : 'true'
    | 'false'
    ;

HEX_LITERAL : '0' ('x'|'X') HexDigit+ ;

DECIMAL_LITERAL : ('0' | '1'..'9' '0'..'9'*) ;

OCTAL_LITERAL : '0' ('0'..'7')+ ;

fragment
HexDigit : ('0'..'9'|'a'..'f'|'A'..'F') ;


FLOATING_POINT_LITERAL
    :   ('0'..'9')+ '.' ('0'..'9')* Exponent?
    |   '.' ('0'..'9')+ Exponent?
    |   ('0'..'9')+ Exponent
    ;

fragment
Exponent : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;


fragment
EscapeSequence
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   OctalEscape
    ;

fragment
OctalEscape
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UnicodeEscape
    :   '\\' 'u' HexDigit HexDigit HexDigit HexDigit
    ;


IDENTIFIER : ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' |'0'..'9' )* ;


QUALIFIEDIDENTIFIER : ('a'..'z' |'A'..'Z' |'_' ) ('a'..'z' |'A'..'Z' |'_' | '.' |'0'..'9' )* ;

COMMENT	: '//' .* '\n' {$channel=HIDDEN;}
        | '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
        ;

WS       : (' '|'\t'|'\n'|'\r')+ {$channel=HIDDEN;} ;
