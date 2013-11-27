// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Lambda.h"
#include "Om/Bin.h"

OM_BEGIN

Executor::Executor() : HasTraceStream(TraceNumber)
{
	break_ = false;
	loop_break = false;
	loop_continue = false;
	EnableTrace(false);	
}

void Executor::SetSourceFile(String const &S)
{
	source_file = S;
	last_file_location.file = source_file.c_str();
}

void Executor::Push(Object const &object)
{
	data.push_back(object);
}

Object Executor::Top() const
{
	if (data.empty())
		OM_THROW_0(EmptyStack);
	return data.back();
}

Object Executor::Pop()
{
	if (data.empty())
		OM_THROW_0(EmptyStack);
	return data.pop();
}

Object Executor::Resolve(Object const &object)
{
	if (!object.IsType<Label>())
		return object;

	Label const &label = Deref<Label>(object);
	if (label.Quoted)
		return object;

	Object resolved = Resolve(label);
	if (!resolved)
	{
		//TraceScopes();
		OM_THROW_1(Unresolved, label);
	}
	return resolved;
}

Object Executor::NeedResolved(Label const &label) const
{
	Object found = Resolve(label);
	if (!found)
		OM_THROW_1(Unresolved, label);
	return found;
}

Object FindIn(Pointer<Continuation> const &cont, Label const &label)
{
	if (!cont || !cont->Scope)
		return Object();
	return cont->Scope.Get(label);
}

Object Executor::Resolve(Label const &label) const
{
	if (Object object = FindIn(continuation, label))
		return object;
	if (!continuation->halt_scope)
	{
		reverse_foreach (Pointer<Continuation> const &cont, context)
		{
			if (Object object = FindIn(cont, label))
				return object;
			if (cont && cont->halt_scope)
				break;
		}
	}
	if (Object object = Self->GetRegistry().GetRoot().Get(label))
		return object;
	if (ClassBase const *klass = Self->GetRegistry().GetClassFromLabel(label))
		return New(klass);
	return null;
}

void Executor::Continue()
{
	while (!context.empty())
	{
		Pointer<Continuation> next = context.pop();
		if (!next)
			break;
		Continue(next);
	}
}

bool IsLiteral(Object const &object)
{
	Type::Number tn = object.GetTypeNumber();
	switch (object.GetTypeNumber().value)
	{
	case Type::Number::Bool:
	case Type::Number::Int:
	case Type::Number::Size:
	case Type::Number::Float:
	case Type::Number::String:
		return true;
	}
	return false;
}

void Executor::Continue(Pointer<Continuation> const &next)
{
	if (!next)
		return;
	Continuation const &next_ref = *next;

	continuation = New<Continuation>();
	Continuation &cont = *continuation;
	cont.Code = next_ref.Code;
	cont.Ip = next_ref.Ip;
	cont.Scope = next_ref.Scope;
	if (!cont.Scope)
		cont.Scope = New<void>();
	
	Vector const &code = *cont.Code;
	for (break_ = false; cont.Ip < code.size(); ++cont.Ip)
	{
		Object const &object = code.at(cont.Ip);
#ifdef _DEBUG
		PreProcess(object);
#endif
		switch (object.GetTypeNumber().value)
		{
		case Type::Number::Operation:
			{
				Perform(Deref<Operation>(object));
			}
			break;
		case Type::Number::Label:
			{
				Push(Resolve(object));
			}
			break;
		default:
			{
				// push copies of literals, so they can be changed
				// TODO: 1 use copy-on-write semantics for literals to avoid copying
				// TODO: 2 special-case for int, bool, string etc
				if (IsLiteral(object))
					Push(Duplicate(object));
				else
					Push(object);
			}
			break;
		}
#ifdef _DEBUG
		OM_TRACE(TraceNumber) << "After:" << data;
#endif
		if (break_)
			break;
	}
}

// this is purely for debugging; keeps track of source-file location for objects
void Executor::PreProcess(Object const &object)
{
	if (!object)
		return;
	StorageBase const &base = object.GetStorage();
	if (base.file_location.line)
	{
		if (base.file_location.file)
			last_file_location.file = base.file_location.file;
		last_file_location.line = base.file_location.line;
		last_file_location.column = base.file_location.column;
	}
	if (HasTraceStream::Enabled())
	{
		Debug::StringStream S;
		S << last_file_location << object << " #" << object.GetHandle().value;
		Debug::Write(S.ToString().c_str());
		TraceScopes();
	}
	OM_TRACE_1(TraceNumber, data);
}

// continue only from the given continuation; when it is done, do not continue down the context stack
void Executor::ContinueOnly(Pointer<Continuation> const &next)
{
	// add a 'null' guard to stop the context unwind when control returns to before `next`
	PushContext(null);
	PushContext(next);
	Continue();
}

Object Executor::ContextPop()
{
	return context.pop();
}

void Executor::PushContext(Pointer<Continuation> const &cont)
{
	context.push_back(cont);
}

void Executor::Break()
{
	break_ = true;
}

Pointer<Continuation> Executor::Current()
{
	++continuation->Ip;
	return continuation;
}

void Executor::OpAssign(Object (*Op)(Object const &, Object const &))
{
	Object B = Pop();
	Object A = Pop();
	Assign(A, Op(A,B));
}

void Executor::Leave()
{
	//continuation->Scope.Delete();
	size_t level = enter_level.back();
	enter_level.pop_back();
	while (data.size() > (int)level)
		data.pop_back();
}

void Executor::Trace(Debug::StringStream &S) const
{
	OM_TRACE_OBJECT_0("Executor", S);//, data, context);
}

void Executor::SetMarked(MarkingContext &mark) const
{
	data.SetMarked(mark);
	context.SetMarked(mark);
}

BinaryStream& operator<<(BinaryStream& S, Executor const &L)
{
	return S << L.data << L.context << L.continuation /*<< L.enter_level*/ << L.break_;
}

BinaryStream& operator>>(BinaryStream& S, Executor &L)
{
	return S >> L.data >> L.context >> L.continuation /*>> L.enter_level */>> L.break_;
}

void Executor::TraceScopes(bool with_code) const
{
	Object root = Self->GetRegistry().GetRoot();
	Debug::StringStream stream;
	int context_number = 0;
	TraceContinuation(context_number++, stream, continuation, with_code);
	reverse_foreach (Pointer<Continuation> const &cont, context)
	{
		TraceContinuation(context_number++, stream, cont, with_code);
	}
	OM_TRACE(0) << last_file_location << endl << stream.ToString();
}

void Executor::EnableTrace(bool E)
{
	HasTraceStream::EnableTrace(E);
}

void Executor::Register(Registry &R)
{
	Object (Executor::*Resolve)(Object const &object) = &Executor::Resolve;
	ClassBuilder<Executor>(R)
		.Methods
			("Top", &Executor::Top)
			("Pop", &Executor::Pop)
			("Push", &Executor::Push)
			("Resolve", Resolve)
			("Perform", &Executor::Perform)
			("EnableTrace", &Executor::EnableTrace)
			("TraceScopes", &Executor::TraceScopes)
		.Properties
			("continuation", &Executor::continuation)
		;
}

OM_END

//EOF
