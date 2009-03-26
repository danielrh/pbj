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
   :   PACKAGELITERAL QUALIFIEDIDENTIFIER ITEM_TERMINATOR
        {
            initPackage( SCOPE_TOP(NameSpace), $QUALIFIEDIDENTIFIER.text );
        }
	;

importrule
   :   IMPORTLITERAL STRING_LITERAL ITEM_TERMINATOR
        {
            initImport( SCOPE_TOP(NameSpace), $STRING_LITERAL.text );
        }
	;


message
    scope {
        pANTLR3_STRING messageName;
    }
    :   MESSAGE message_identifier BLOCK_OPEN message_elements BLOCK_CLOSE
        {
            defineType( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $message::messageName );
            stringFree($message::messageName);
        }
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
	;

enum_def
    scope {
        pANTLR3_STRING enumName;
        pANTLR3_LIST enumList;
    }
    @init {
        $enum_def::enumList=antlr3ListNew(1);
    }
	:	ENUM enum_identifier BLOCK_OPEN enum_element+ BLOCK_CLOSE
        {
            defineEnum( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $enum_def::enumName, $enum_def::enumList);
            $enum_def::enumList->free($enum_def::enumList);
            stringFree($enum_def::enumName);
        }
	;

enum_element
	:	IDENTIFIER EQUALS integer ITEM_TERMINATOR
        {
            defineEnumValue( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $enum_def::enumName, $enum_def::enumList, $IDENTIFIER.text, $integer.text );
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
    }
    @init {
        $flags_def::flagList=antlr3ListNew(1);
        
    }
	:	FLAGS flag_identifier BLOCK_OPEN flag_element+ BLOCK_CLOSE
        {
            defineFlag( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $flags_def::flagName, $flags_def::flagList);
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
	:	IDENTIFIER EQUALS integer ITEM_TERMINATOR
        {
            defineFlagValue( SCOPE_TOP(NameSpace), SCOPE_TOP(Symbols), $flags_def::flagName, $flags_def::flagList, $IDENTIFIER.text , $integer.text);
        }
	;

field
    scope{
        pANTLR3_STRING fieldType;
        pANTLR3_STRING fieldName;
        int fieldOffset;
    }
    :   ( (OPTIONAL multiplicitive_type field_name EQUALS field_offset default_value? ITEM_TERMINATOR ) | ( (REQUIRED|REPEATED) multiplicitive_type field_name EQUALS field_offset ITEM_TERMINATOR ) ) -> REPEATED["repeated"] multiplicitive_type field_name EQUALS field_offset ITEM_TERMINATOR 
     |
      ( (OPTIONAL field_type field_name EQUALS field_offset default_value? ITEM_TERMINATOR ) | ( (REQUIRED|REPEATED) field_type field_name EQUALS field_offset ITEM_TERMINATOR ) )
    {
        //printf ("Field name: \%s \%s=\%d\n",$field::fieldType->chars,$field::fieldName->chars,$field::fieldOffset);
        stringFree($field::fieldName);
        stringFree($field::fieldType);
    }
	;

field_offset
    : integer
    {
        
        $field::fieldOffset=atoi($integer.text->chars);
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
    | IDENTIFIER
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

type:	UINT8
	|	INT8
	|	SINT8
	|	FIXED8
	|	SFIXED8
	|	UINT16
	|	INT16
	|	SINT16
	|	FIXED16
	|	SFIXED16
	|	UINT32
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
	|	BOOL

	;
multiplicitive_advanced_type:
    |   NORMAL3F -> FLOAT["float"]
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
    ;

advanced_type:	BYTE -> BYTES["bytes"]
	|	FLAGS -> ENUM["enum"]
    |   UUID -> BYTES["bytes"]
    |   ANGLE -> FLOAT["float"]
    |   TIME -> FIXED64["fixed64"]
    |   DURATION -> SFIXED64["sfixed64"]
    |   BOUNDINGBOX3D3F
    ; 
bookey : BOOKEY_LITERAL ;
BOOKEY_LITERAL :    'bookey';
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

// Enum elements
ENUM	:	'enum';

// Flags elements
FLAGS	:	'flags';

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
NORMAL3F : 'normal3f';
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
