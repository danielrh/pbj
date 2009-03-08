grammar PBJ;

options
{
    language = C;
}

protocol
	:	message*
	;

message :	MESSAGE IDENTIFIER BLOCK_OPEN message_element* BLOCK_CLOSE
	;

message_element
	:	field
	|	message
	|	enum_def
	;

enum_def
	:	ENUM IDENTIFIER BLOCK_OPEN enum_element+ BLOCK_CLOSE
	;

enum_element
	:	IDENTIFIER EQUALS integer ITEM_TERMINATOR
	;

flags_def
	:	FLAGS IDENTIFIER BLOCK_OPEN BLOCK_CLOSE
	;

field
    :   OPTIONAL (type|IDENTIFIER) IDENTIFIER EQUALS integer default_value? ITEM_TERMINATOR
	|	(REQUIRED|REPEATED) (type|IDENTIFIER) IDENTIFIER EQUALS integer ITEM_TERMINATOR
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
	|	FLAGS
	;

literal_value
	:	HEX_LITERAL
    |   DECIMAL_LITERAL
    |   OCTAL_LITERAL
    |   FLOATING_POINT_LITERAL
    |   BOOL_LITERAL
    |   STRING_LITERAL
    ;

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


COMMENT	: '//' .* '\n' {$channel=HIDDEN;}
        | '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
        ;

WS       : (' '|'\t'|'\n'|'\r')+ {$channel=HIDDEN;} ;
