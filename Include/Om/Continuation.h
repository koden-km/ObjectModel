// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// a continuation represents the 'rest of the program' from a given location
struct Continuation : Debug::ITraceable
{
	Pointer<Vector> Code;
	Pointer<Object> Scope;
	size_t Ip;
	bool halt_scope;

	Continuation() : halt_scope(false) { }

	void Set(Pointer<Vector>, Object = Object());

	void Enter();
	void Leave();

	void Trace(Debug::StringStream &) const;
	static void Register(Registry &);
};

OM_TRAITS_OPS(Continuation, Operations::Assign | Operations::StringInsert);

// specialised trace used by executor
void TraceContinuation(int number, Debug::StringStream &S, Pointer<Continuation> const &cont, bool with_code = false);

OM_END

//EOF
