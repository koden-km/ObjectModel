// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

tree grammar Tree;

options 
{
	tokenVocab = Rho;				// reuse token types
	ASTLabelType = pANTLR3_BASE_TREE;	// $label will have type CommonTree
	language = C;
}

scope CodeSequence
{
	Om::Pointer<Om::Vector> code;
}

@header
{
	#include "Precompiled.h"
}

@members
{
	#include "Om/Field.h"
	#include "Om/Reflected.h"
	#include "Om/Compiler.h"
	
	OM_USING_NAMESPACE_OM();
	
	extern void Trace(const char *, ...);
	static const char * getGrammarFileName();
	
	void AddDebugInfo(Object &Q, pANTLR3_BASE_TREE p)
	{
		if (p)
		{
			StorageBase &base = Q.GetStorage();
			//base.file_location.file = getGrammarFileName();
			base.file_location.line = (p->getLine(p));
			base.file_location.column = (p->getCharPositionInLine(p));
		}
	}

	template <class T>
	void Append(T const &X, pANTLR3_BASE_TREE tree = 0)
	{
		Pointer<T> Q = compiler->New(X);
		AddDebugInfo(Q, tree);
		compiler->Append(Q);
	}
	
	void AppendOp(Operation::Type op, pANTLR3_BASE_TREE tree = 0)
	{
		Object Q = compiler->New(Operation(op));
		AddDebugInfo(Q, tree);
		compiler->Append(Q);
	}
	
	String StripQuotes(const std::string &S)
	{
		return S.substr(1, S.size() - 2);
	}
	
	void AppendIntLit(const char *chars, pANTLR3_BASE_TREE tree = 0)
	{
		String S = chars;
		char postfix = S[S.size() - 1];
		if (postfix == 'l' || postfix == 'L')
			Append(size_t(atol(chars)), tree);
		else
			Append(atoi(chars), tree);
	}
}

//------------------------------------------------------------------------------------------------

entry
	:	statement+
	;
	
continuation
	:	{ compiler->Push(); } statement { compiler->PopContinuation(); } 
	;

statement
	:	expression
	|	^(q=IF_ELSE continuation continuation expression)	{ AppendOp(Operation::IfThenSuspendElseSuspend, q); }
	|	^(q=IF  continuation expression)					{ AppendOp(Operation::IfThenSuspend, q); }
	|	^(q='for' cond=continuation step=continuation body=continuation expression)	{ AppendOp(Operation::ForLoop, q); }
	|	block
	|	^(a=ASSERT e=expression)			{ AppendOp(Operation::Assert, $a); }
	//|	^(ASSERT expression expression)		{ AppendOp(Operation::Assert); }
	;

// a block resets the stack to the level that it was when the block was entered
block
	:	{ AppendOp(Operation::Enter); } ^(BLOCK statement*) { AppendOp(Operation::Leave); } 
	;
	

expression
	:	^(q='=' expression expression)					{ AppendOp(Operation::Assign, q); }
	|	^(q=LAMBDA ident_list ident_list continuation)	{ compiler->AddLambda(); }
	|	'this'								{ AppendOp(Operation::This); }
	|	^(q=':=' expression expression)		{ AppendOp(Operation::Store, q); }
	|	^(q='==' expression expression)		{ AppendOp(Operation::Equiv, q); }
	|	^(q='<>' expression expression)		{ AppendOp(Operation::NotEquiv, q); }
	|	^(q='<' expression expression)		{ AppendOp(Operation::Less, q); }
	|	^(q='+' expression expression)		{ AppendOp(Operation::Plus, q); }
	|	^(q='-' expression expression)		{ AppendOp(Operation::Minus, q); }
	|	^(q='*' expression expression)		{ AppendOp(Operation::Multiply, q); }
	|	^(q='/' expression expression)		{ AppendOp(Operation::Divide, q); }
	|	^(q='%' expression expression)		{ AppendOp(Operation::Modulo, q); }
	|	^(q='||' expression expression)		{ AppendOp(Operation::LogicalOr, q); }
	|	^(q='&&' expression expression)		{ AppendOp(Operation::LogicalAnd, q); }
	|	^(q='!' expression)					{ AppendOp(Operation::LogicalNot, q); }
	|	^(q='new' ident)						{ AppendOp(Operation::New, q); }
	|	^(q='delete' expression)				{ AppendOp(Operation::Delete, q); }
	|	^(q='exists' expression)				{ AppendOp(Operation::Exists, q); }
	|	^(q='trace' expression)				{ AppendOp(Operation::Trace, q); }
	|	^(q='++' expression)					{ AppendOp(Operation::PreInc, q); }
	|	^(q='+=' expression expression)		{ AppendOp(Operation::PlusAssign, q); }
	|	^(q='-=' expression expression)		{ AppendOp(Operation::MinusAssign, q); }
	|	^(q='*=' expression expression)		{ AppendOp(Operation::MultiplyAssign, q); }
	|	^(q='/=' expression expression)		{ AppendOp(Operation::DivideAssign, q); }
	|	q='_break'							{ AppendOp(Operation::DebugBreak, q); }
	|	q='break'							{ AppendOp(Operation::Break, q); }
	|	q='continue'							{ AppendOp(Operation::Continue, q); }
	|	^(q='return' expression?)				{ AppendOp(Operation::Return, q); }
	|	^(RPN_SEQUENCE rpn_token*)
	|	literal
	|	ident
	|	^(q=METHOD_CALL arguments expression)			{ AppendOp(Operation::MethodCall, q); }
	|	^(q='.' expression id=Identifier)				{ Append(Label((const char *)$id.text->chars, true)); AppendOp(Operation::GetProperty, q); }
	|	^(q='->' expression id=Identifier)			{ AppendOp(Operation::GetProperty, q); }
	//|	^('->' expression QuotedIdentifier)			{ AppendOp(Operation::GetProperty); }
	;

ident
	:	id=Identifier				{ Append(Label((const char *)$id.text->chars), id); }
	|	id2=QuotedIdentifier		{ Append(Label((const char *)$id2.text->chars, true), id); }
	;
	
arguments
	:	^(ARG_LIST expression*)
	;
	
ident_list
	:	{ compiler->Push(); } ^(IDENT_LIST ident*) { compiler->PopSequence(); }
	;
	
literal	
	:	n=integerLiteral				{ AppendIntLit((const char *)$n.text->chars); }
	|	f=FloatingPointLiteral		{ Append((float)atof((const char *)$f.text->chars), f); }
	|	CharacterLiteral
	|	s=StringLiteral				{ Append(StripQuotes((const char *)$s.text->chars), s); }
	|	b=booleanLiteral			{ Append(String((const char *)$b.text->chars) == "true"); }
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


operation
	:	q='+'		{ AppendOp(Operation::Plus, q); }
	|	q='-'		{ AppendOp(Operation::Minus, q); }
	|	q='*'		{ AppendOp(Operation::Multiply, q); }
	|	q='/'		{ AppendOp(Operation::Divide, q); }
	|	q=':='		{ AppendOp(Operation::Store, q); }
	|	q='='		{ AppendOp(Operation::Assign, q); }
	|	q='=='		{ AppendOp(Operation::Equiv, q); }
	|	q='!='		{ AppendOp(Operation::Equiv, q); AppendOp(Operation::LogicalNot, q);}
	|	q='<'		{ AppendOp(Operation::Less, q); }
	|	q='drop'	{ AppendOp(Operation::Drop, q); }
	|	q='swap'	{ AppendOp(Operation::Swap, q); }
	|	q='clear'	{ AppendOp(Operation::Clear, q); }
	|	q='break'	{ AppendOp(Operation::DebugBreak, q); }
	|	q='quit'	{ AppendOp(Operation::Quit, q); }
	|	q='dup'		{ AppendOp(Operation::Dup, q); }
	|	q='new'		{ AppendOp(Operation::New, q); }
	|	q='trace'	{ AppendOp(Operation::Trace, q); }
	|	q='type'	{ AppendOp(Operation::Typeof, q); }
	|	q='assert'	{ AppendOp(Operation::Assert, q); }
	|	q='&'		{ AppendOp(Operation::Suspend, q); }
	|	q='!'		{ AppendOp(Operation::Replace, q); }
	|	q='...'		{ AppendOp(Operation::Resume, q); }
	|	q='?!'		{ AppendOp(Operation::IfThenReplace, q); }
	|	q='?&'		{ AppendOp(Operation::IfThenSuspend, q); }
	|	q='?...'	{ AppendOp(Operation::IfThenResume, q); }
	|	q='?!!'		{ AppendOp(Operation::IfThenReplaceElseReplace, q); }
	|	q='?&&'		{ AppendOp(Operation::IfThenSuspendElseSuspend, q); }
	|	q='?&..'	{ AppendOp(Operation::IfThenSuspendElseResume, q); }
	;

rpn_token
	:	literal
	|	operation
	|	q=Identifier			{ Append(Label((const char *)$Identifier.text->chars), q); }
	|	q=QuotedIdentifier		{ Append(Label((const char *)$QuotedIdentifier.text->chars, true), q); }
	|	^(CONTINUATION {compiler->Push(); } rpn_token* { compiler->PopContinuation(); } )		
	;
	
//EOF
