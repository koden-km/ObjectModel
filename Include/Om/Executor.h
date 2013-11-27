// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Continuation.h"

OM_BEGIN

/// processes code streams
struct Executor : Debug::ITraceable, IMarkable, Reflected<Executor>, HasTraceStream
{
	typedef Vector Stack;
	enum { TraceNumber = 8 };

private:
	Stack data;
	Stack context;
	std::list<size_t> enter_level;
	bool break_;
	bool loop_break, loop_continue;

	Field<Continuation> continuation;
	FileLocation last_file_location;
	String source_file;

public:
	Executor();

	void SetSourceFile(String const &S);
	void PushContext(Pointer<Continuation> const &);
	void Continue();
	void Continue(Pointer<Continuation> const &);
	void ContinueOnly(Pointer<Continuation> const &C);
	Object ContextPop();
	void EnableTrace(bool E);

	FileLocation const &GetLastFileLocation() const { return last_file_location; }
	Object Top() const;
	Object Pop();
	void Push(Object const &);
	Pointer<Continuation> Current();

	Object ResolvePop() { return Resolve(Pop()); }
	Object Resolve(Object const &object);
	Object Resolve(Label const &) const;
	Stack const &GetDataStack() const { return data; }
	Object NeedResolved(Label const &) const;

	void Trace(Debug::StringStream &) const;
	virtual void SetMarked(MarkingContext &mark) const;
	static void Register(Registry &);

	void Perform(Operation op);

	void TraceScopes(bool with_code = false) const;

	friend BinaryStream& operator<<(BinaryStream& S, Executor const &L);
	friend BinaryStream& operator>>(BinaryStream& S, Executor &L);

protected:

	friend struct Lambda;

	void Suspend(Pointer<Vector> const &);
	void Suspend(Pointer<Continuation> const &);

	void Break();
	void Leave();

	template <class Ret, class A0, class A1>
	void BinaryOpRet(Ret (ClassBase::*method)(A0, A1) const);

	template <class A0, class A1>
	void BinaryOpVoid(void (ClassBase::*method)(A0, A1) const);

	template <class A0, class A1>
	void BinaryOp(Object (ClassBase::*method)(A0, A1) const);

	void OpAssign(Object (*Op)(Object const &, Object const &));

	void PreProcess(Object const &);
};

BinaryStream& operator<<(BinaryStream&, Executor const &);
BinaryStream& operator>>(BinaryStream&, Executor &);

OM_TRAITS_OPS(Executor, Operations::Streaming);

OM_END

//EOF
