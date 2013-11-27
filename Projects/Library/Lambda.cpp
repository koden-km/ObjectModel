// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Continuation.h"
#include "Om/Lambda.h"

OM_BEGIN

void Lambda::Enter(Executor &executor) const
{
	// make new scope object for lambda
	Object scope = executor.New<void>();

	// capture objects from existing scope
	foreach (Label const &label, capture)
	{
		scope[label] = DeepCopy(executor.NeedResolved(label));
	}

	// pull arguments from stack
	reverse_foreach (Label const &label, arguments)
	{
		Object arg = executor.Pop();
		scope[label] = arg;
	}

	// return to current context after lambda completes
	executor.PushContext(executor.Current());

	// TODO: there is also a copy in executor.Continue...
	Pointer<Continuation> copy = Duplicate(continuation);
	copy->Scope = scope;

	// stop name resolution at this scope; dont check further up the context stack after this
	copy->halt_scope = true;		
	executor.PushContext(copy);
	executor.Break();
}

void Lambda::Trace(Debug::StringStream &S) const
{
	OM_TRACE_OBJECT_3("Lambda", S, capture, arguments, continuation);
}

void Lambda::Archive(BinaryStream &)
{
}

BinaryStream &operator<<(BinaryStream &, Lambda const &)
{
	OM_NOT_IMPLEMENTED();
}

BinaryStream &operator>>(BinaryStream &, Lambda &)
{
	OM_NOT_IMPLEMENTED();
}

void Lambda::Register(Registry &R)
{
	ClassBuilder<Lambda>(R)
		.Methods
		.Properties
			("continuation", &Lambda::continuation)
		;
}

OM_END

//EOF
