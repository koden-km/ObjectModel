// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Continuation.h"

OM_BEGIN

void Continuation::Set(Pointer<Vector> C, Object S)
{
	Code = C;
	Scope = S;
	Ip = 0;
}

void Continuation::Enter()
{
}

void Continuation::Leave()
{
}

void Continuation::Trace(Debug::StringStream &S) const
{
	OM_TRACE_OBJECT_3("", S, Code, Scope, Ip);
}

void Continuation::Register(Registry &R)
{
	ClassBuilder<Continuation>(R)
		.Methods
		.Properties
			//("Code", &Continuation::Code)
			//("Scope", &Continuation::Scope)
			//("Ip", &Continuation::Ip)
		;
}

// specialised trace for executor debugging
void TraceContinuation(int number, Debug::StringStream &S, Pointer<Continuation> const &cont, bool with_code)
{
	S << "[" << number << "]: ";
	if (!cont)
	{
		S << "null continuation" << endl;
		return;
	}
	S << "cont#" << cont.GetHandle().value << ": ";
	if (!cont->Scope)
	{
		S << "empty scope";
	}
	else
	{
		std::vector<String> idents;
		cont->Scope.ForEachChild([&idents](StorageBase const &obj) { idents.push_back(obj.GetIdent()); });
		foreach_delim_(String const &ident, idents, ", ", S << ident);
	}
	if (!with_code)
	{
		S << endl;
		return;
	}
	S << indent << endl;
	int ip = 0;
	foreach (Object const &object, *cont->Code)
	{
		if (ip == cont->Ip)
			S << ">>>>>> ";
		S << object << endl;
		++ip;
	}
	S << outdent << endl;
}

OM_END

//EOF
