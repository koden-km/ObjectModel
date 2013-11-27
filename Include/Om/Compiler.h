// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Utility/NonCopyable.h"

OM_BEGIN

OM_EXCEPTION_1(CompileError, String, error_text);

/// hides implementation details to remove dependencies on compiler internals
struct CompilerImpl;

/// compiles text or a text-file to a stream of Objects to be processed by an Executor
struct Compiler : Reflected<Compiler>, Debug::ITraceable, HasTraceStream, NonCopyable
{
	enum { TraceNumber = 7 };

private:
	CompilerImpl *impl;

	Field<Vector> stack;
	Field<Vector> code;

public:
	Compiler();
	~Compiler();

	void Construct();

	Field<bool> PrintTree;
	Field<bool> Debug;

	Object/*Pointer<Continuation>*/ CompileFile(String const &filename);
	Object/*Pointer<Continuation>*/ CompileText(String const &text);

	void Reset();
	void Push();
	void Append(Object const &);
	void PopSequence();
	void PopContinuation();
	void Pop();

	void Trace(Debug::StringStream &S) const;	
	static void Register(Registry &);

	void AddLambda();
	
protected:
	/// perform lexical analysis, syntax parsing and AST (Abstract Syntax Tree) traversal to produce an executable object stream
	Pointer<Continuation> LexParseWalk();
	Compiler(Compiler const &);
	Compiler &operator=(Compiler const &);
};

OM_TRAITS(Compiler);

/// the global compiler object
extern Pointer<Compiler> compiler;

OM_END

//EOF
