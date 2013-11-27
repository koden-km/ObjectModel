// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

grammar Rho;

options
{
	language = C;
	output = AST;
	//k = 2; 
	backtrack = true; 
	memoize = true;
}


tokens
{
	METHOD_CALL;
	FIELD_ACCESS;
	FUNCTION_CALL;
	ARG_LIST;
	IDENT_LIST;
	ASSIGN;
	BLOCK;
	EMPTY;
	RPN_SEQUENCE;
	QUOTED_RPN_SEQUENCE;
	IF;
	IF_ELSE;
	ELSE;
	ASSERT;
	EXISTS;
	CONTINUATION;
	LAMBDA;
	DEBUG_BREAK;
	BREAK;
	TRACE;
}

@header
{
	// this is included in parser.h
}

@parser::includes
{

}

@members
{
	void Trace(const char *, ...);
}

// --------------------------------------------------------------------------------

entry
	:	statement+
	;
	
block
	:	'{' blockStatement* '}'	-> ^(BLOCK blockStatement*)
	;
	
blockStatement
//	:	localVariableDeclaration
//	|	classOrInterfaceDeclaration
	:	statement
	;
	
statement
	:	block
	|	if_block
	|	'@' rpn_token -> ^(RPN_SEQUENCE rpn_token)
	|	'delete'^ expression
	
	|	'for' '(' start=expression? ';' cond=expression? ';' next=expression? ')' body=statement
		-> ^('for' $cond $next $body $start )
	|	lc='assert' e=parExpression ';' -> ^(ASSERT[$lc] $e)
	//|	'while' parExpression statement
	//|	'do' statement 'while' parExpression ';'
	//|	'try' block
	//	(	
	//		catches 'finally' block
	//	|	catches
	//	|	'finally' block
	//	)
	//|	'switch' parExpression '{' switchBlockStatementGroups '}'
	//|	'synchronized' parExpression block
	|	'return' expression? ';' -> ^('return' expression)
	//|	'throw' expression ';'
	|	'_break' ';'!
	|	'break' ';'!
	|	'continue' ';'!
	|	';'!
	|	statementExpression ';'!
	//|	Identifier ':' statement
	;
	
// TODO: there is a more efficient way to do this, and still generate the correct AST tree
if_block
	:	'if' exp=parExpression true_=statement 'else' false_=statement -> ^(IF_ELSE $true_ $false_ $exp)
	|	'if' exp=parExpression true_=statement -> ^(IF $true_ $exp)
	;	
	
statementExpression
	:	expression
	;
	
nonWildcardTypeArguments
	:	'<' typeList '>'
	;

typeList
	:	type (',' type)*
	;

packageOrTypeName
	:	Identifier ('.' Identifier)*
	;

typeName
	:	Identifier
	|	packageOrTypeName '.' Identifier
	;

type
	:	Identifier (typeArguments)? ('.' Identifier (typeArguments)? )* // ('[' ']')*
	|	primitiveType // ('[' ']')*
	;
	
typeArguments
	:	'<' typeArgument (',' typeArgument)* '>'
	;

typeArgument
	:	type
	//|	'?' (('extends' | 'super') type)?
	;
	
primitiveType
	:	'bool'
	|	'char'
	|	'byte'
	|	'short'
	|	'int'
	|	'long'
	|	'float'
	|	'double'
	
	|	'string'
	|	'wstring'
	
	|	'float2'
	|	'float3'
	|	'float4'
	|	'float2x2'
	|	'float3x3'
	|	'float4x4'
	;

literal	
	:	integerLiteral
	|	FloatingPointLiteral
	|	CharacterLiteral
	|	StringLiteral
	|	booleanLiteral
	|	'null'
	;

integerLiteral
	:	HexLiteral
	|	OctalLiteral
	|	DecimalLiteral
	;

booleanLiteral
	:	'true'
	|	'false'
	;

//---------------------------------------------------------------------------------
//					 e x p r e s s i o n
//---------------------------------------------------------------------------------

expressionList
	:	expression (','! expression)*
	;

expression
	:	conditionalExpression (assignmentOperator^ expression)?
	;
	
assignmentOperator
	:	'='
	|	':='
	|	'+='
	|	'-='
	|	'*='
	|	'/='
	|	'&='
	|	'|='
	|	'^='
	|	'%='
	;

conditionalExpression
	:	conditionalOrExpression ( '?' expression ':' expression )?
	;

conditionalOrExpression
	:	conditionalAndExpression ( '||'^ conditionalAndExpression )*
	;

conditionalAndExpression
	:	inclusiveOrExpression ( '&&'^ inclusiveOrExpression )*
	;

inclusiveOrExpression
	:	exclusiveOrExpression ( '|'^ exclusiveOrExpression )*
	;

exclusiveOrExpression
	:	andExpression ( '^'^ andExpression )*
	;

andExpression
	:	equalityExpression ( '&'^ equalityExpression )*
	;

equalityExpression
	:	instanceOfExpression ( ('=='^ | '<>'^) instanceOfExpression )*
	;

instanceOfExpression
	:	relationalExpression ('instanceof'^ type)?
	;

relationalExpression
	:	shiftExpression ( relationalOp^ shiftExpression )*
	;
	
relationalOp
	:	('<' '=' | '>' '=' | '<' | '>')
	;

shiftExpression
	:	additiveExpression ( shiftOp^ additiveExpression )*
	;

		// TODO: need a sem pred to check column on these >>>
shiftOp
	:	('<' '<' | '>' '>')
	;


additiveExpression
	:	multiplicativeExpression ( ('+'^ | '-'^) multiplicativeExpression )*
	;

multiplicativeExpression
	:	unaryExpression ( ( '*'^ | '/'^ | '%'^ ) unaryExpression )*
	;
	
unaryExpression
	:	'+'^ unaryExpression
	|	'-'^ unaryExpression
	|	'++'^ unaryExpression
	|	'--'^ unaryExpression
	|	unaryExpressionNotPlusMinus
	;

unaryExpressionNotPlusMinus
	:	'~' unaryExpression
	| 	'!'^ unaryExpression
	|	castExpression
	|	postfixExpression ('++'^|'--'^)?
	|	'new'^ Identifier// arguments
	|	'this'
	|	'exists'^ parExpression
	|	'trace'^ parExpression
	|	rpn_block
	;
	
selector
	:	'.' id=Identifier
	|	'.' id=Identifier arguments?
	//|	'.' 'this'
	//|	'.' 'super' superSuffix
	//|	'.' 'new' (nonWildcardTypeArguments)? innerCreator
	//|	'[' expression ']'
	;

innerCreator
	:	Identifier
	;
	
castExpression
	:  '(' primitiveType ')' unaryExpression
	|  '(' (type | expression) ')' unaryExpressionNotPlusMinus
	;

parExpression
	:	'(' expression ')' -> expression
	;
	
primary
	:	parExpression
	|	literal
	|	QuotedIdentifier
	|	Identifier
	|	lambda_definition
	;

lambda_definition
	:	capture_list arg_list block -> ^(LAMBDA capture_list arg_list block)
	;
	
capture_list
	:	'[' ident_list? ']' -> ^(IDENT_LIST ident_list)
	;

arg_list
	:	'(' ident_list? ')' -> ^(IDENT_LIST ident_list)
	;
	
ident_list
	:	Identifier (','! Identifier)*
	;
	
postfixExpression
	:	(primary->primary) // set return tree to just primary
		( arguments
			-> ^(METHOD_CALL["method_call"] arguments $postfixExpression)
//		|	'[' ie=expression ']'
//			-> ^(INDEX $postfixExpression $ie)
		| '.' p=primary
			-> ^('.' $postfixExpression $p)
		| '->' p=primary
			-> ^('->' $postfixExpression $p)
		)*
	;
	
creator
//	:	nonWildcardTypeArguments? createdName
  //	  (arrayCreatorRest | classCreatorRest)
	//;
	:	createdName arguments
	;

createdName
	:	Identifier typeArguments?
		('.' Identifier typeArguments?)*
	|	primitiveType
	;

identifierSuffix
	//:	('[' ']')+ '.' 'class'
	//|	('[' expression ']')+ // can also be matched by selector, but do here
	:	arguments
	|	'.' 'class'
	//|	'.' explicitGenericInvocation
	|	'.' 'this'
	|	'.' 'super' arguments
	//|	'.' 'new' (nonWildcardTypeArguments)? Identifier //innerCreator
	;
	
arguments
	:	'(' expressionList? ')' -> ^(ARG_LIST expressionList)
	;

rpn_block
	:	'rpn' '[' rpn_token* ']' -> ^(RPN_SEQUENCE rpn_token*)
	;
	
rpn_token
	:	literal
	|	Identifier
	|	QuotedIdentifier
	|	operation
	|	continuation
	;
	
continuation
	:	'{' rpn_token* '}' -> ^(CONTINUATION rpn_token*)
	;

operation
	:	'+'
	|	'-'
	|	'*'
	|	'/'
	|	':='
	|	'='
	|	'=='
	|	'!='
	|	'<'
	|	'&'
	|	'!'
	|	'...'
	|	'drop'
	|	'swap'
	|	'clear'
	|	'quit'
	|	'dup'
	|	'new'
	|	'type'
	|	'assert'
	|	'break'
	|	'?!'
	|	'?&'
	|	'?...'
	|	'?!&'
	|	'?&&'
	|	'?...&'
	|	'?!!'
	|	'?&!'
	|	'?...!'
	|	'?!...'
	|	'?&...'
	;

//--------------------------------------------------------------------
//						L E X E R 
//--------------------------------------------------------------------

HexLiteral : '0' ('x'|'X') HexDigit+ IntegerTypeSuffix? ;

DecimalLiteral : ('0' | '1'..'9' '0'..'9'*) IntegerTypeSuffix? ;

OctalLiteral : '0' ('0'..'7')+ IntegerTypeSuffix? ;

fragment
HexDigit : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
IntegerTypeSuffix : ('l'|'L') ;

FloatingPointLiteral
	:	('0'..'9')+ '.' ('0'..'9')* Exponent? FloatTypeSuffix?
	|	'.' ('0'..'9')+ Exponent? FloatTypeSuffix?
	|	('0'..'9')+ (	  Exponent FloatTypeSuffix?
						| FloatTypeSuffix
					)
	;

fragment
Exponent : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
FloatTypeSuffix : ('f'|'F'|'d'|'D') ;

CharacterLiteral
	:	'\'' ( EscapeSequence | ~('\''|'\\') ) '\''
	;

StringLiteral
	:  '"' ( EscapeSequence | ~('\\'|'"') )* '"'
	;

fragment
EscapeSequence
	:	'\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
	|	UnicodeEscape
	|	OctalEscape
	;

fragment
OctalEscape
	:	'\\' ('0'..'3') ('0'..'7') ('0'..'7')
	|	'\\' ('0'..'7') ('0'..'7')
	|	'\\' ('0'..'7')
	;

fragment
UnicodeEscape
	:	'\\' 'u' HexDigit HexDigit HexDigit HexDigit
	;

Identifier 
	:	Letter (Letter|JavaIDDigit)*
	;

QuotedIdentifier 
	:	'`' Identifier
	;

/**I found this char range in JavaCC's grammar, but Letter and Digit overlap.
	Still works, but...
 */
fragment
Letter
	:  '\u0024' |
		'\u0041'..'\u005a' |
		'\u005f' |
		'\u0061'..'\u007a' |
		'\u00c0'..'\u00d6' |
		'\u00d8'..'\u00f6' |
		'\u00f8'..'\u00ff' |
		'\u0100'..'\u1fff' |
		'\u3040'..'\u318f' |
		'\u3300'..'\u337f' |
		'\u3400'..'\u3d2d' |
		'\u4e00'..'\u9fff' |
		'\uf900'..'\ufaff'
	;

fragment
JavaIDDigit
	:  '\u0030'..'\u0039' |
		'\u0660'..'\u0669' |
		'\u06f0'..'\u06f9' |
		'\u0966'..'\u096f' |
		'\u09e6'..'\u09ef' |
		'\u0a66'..'\u0a6f' |
		'\u0ae6'..'\u0aef' |
		'\u0b66'..'\u0b6f' |
		'\u0be7'..'\u0bef' |
		'\u0c66'..'\u0c6f' |
		'\u0ce6'..'\u0cef' |
		'\u0d66'..'\u0d6f' |
		'\u0e50'..'\u0e59' |
		'\u0ed0'..'\u0ed9' |
		'\u1040'..'\u1049'
	;

WS  :  (' '|'\r'|'\t'|'\u000C'|'\n') {$channel=HIDDEN;}
	;

COMMENT
	:	'/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
	;

LINE_COMMENT
	:	'//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
	;

// EOF
