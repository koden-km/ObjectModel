// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Operation.h"

OM_BEGIN

Object Plus(Object const &A, Object const &B)
{
	return A.GetClass()->Plus(A,B);
}

Object Minus(Object const &A, Object const &B)
{
	return A.GetClass()->Minus(A,B);
}

Object Divide(Object const &A, Object const &B)
{
	return A.GetClass()->Divide(A,B);
}

Object Multiply(Object const &A, Object const &B)
{
	return A.GetClass()->Multiply(A,B);
}

Object Dereference(Object const &A)
{
	OM_THROW(NotImplemented);
}

Object Duplicate(Object const &A)
{
	return A.Clone();
}

void Assign(Object &A, Object const &B)
{
	A.GetClass()->Assign(A,B);
}

bool Equiv(Object const &A, Object const &B)
{
	return A.GetClass()->Equiv(A,B);
}

bool Less(Object const &A, Object const &B)
{
	return A.GetClass()->Less(A,B);
}

bool Not(Object const &A)
{
	return A.GetClass()->Not(A);
}

bool Boolean(Object const &A)
{
	return A.GetClass()->Boolean(A);
}

BinaryStream& operator<<(BinaryStream& S, Operation const &L)
{
	OM_NOT_IMPLEMENTED();
}

BinaryStream& operator>>(BinaryStream& S, Operation &L)
{
	OM_NOT_IMPLEMENTED();
}

String Operation::ToString() const
{
	switch (type)
	{
	case None: return "None";
	case Typeof: return "Typeof";
	case New: return "New";
	case Delete: return "Delete";
	case This: return "This";
	case Assign: return "Assign";
	case Equiv: return "Equiv";
	case Less: return "Less";
	case Plus: return "Plus";
	case Minus: return "Minus";
	case Divide: return "Divide";
	case Multiply: return "Multiply";
	case GetProperty: return "GetProperty";
	case MethodCall: return "MethodCall";
	case FunctionCall: return "FunctionCall";
	case Drop: return "Drop";
	case Dup: return "Dup";
	case Clear: return "Clear";
	case DebugBreak: return "DebugBreak";
	case Quit: return "Quit";
	case Enter: return "Enter";
	case Leave: return "Leave";
	case Swap: return "Swap";
	case Rot: return "Rot";
	case RotN: return "RotN";
	case Roll: return "Roll";
	case RollN: return "RollN";
	case PickN: return "PickN";
	case Retrieve: return "Retrieve";
	case Store: return "Store";
	case Name: return "Name";
	case LogicalOr: return "LogicalOr";
	case LogicalAnd: return "LogicalAnd";
	case Assert: return "Assert";
	case Exists: return "Exists";
	case IfThenSuspend: return "IfThenSuspend";
	case IfThenReplace: return "IfThenReplace";
	case IfThenResume: return "IfThenResume";
	case IfThenSuspendElseSuspend: return "IfThenSuspendElseSuspend";
	case IfThenReplaceElseSuspend: return "IfThenReplaceElseSuspend";
	case IfThenResumeElseSuspend: return "IfThenResumeElseSuspend";
	case IfThenSuspendElseReplace: return "IfThenSuspendElseReplace";
	case IfThenReplaceElseReplace: return "IfThenReplaceElseReplace";
	case IfThenResumeElseReplace: return "IfThenResumeElseReplace";
	case IfThenSuspendElseResume: return "IfThenSuspendElseResume";
	case IfThenReplaceElseResume: return "IfThenReplaceElseResume";
	case IfThenResumeElseResume: return "IfThenResumeElseResume";

	case Suspend: return "Suspend";
	case Replace: return "Replace";
	case Resume: return "Resume";

	case PreInc: return "PreInc";
	case PlusAssign: return "PlusAssign";
	case MinusAssign: return "MinusAssign";
	case MultiplyAssign: return "MultiplyAssign";
	case DivideAssign: return "DivideAssign";
	case ForLoop: return "ForLoop";
	case Modulo: return "Modulo";
	case Return: return "Return";
	case Break: return "Break";
	case Continue: return "Continue";
	case Trace: return "Trace";
	}
	return "?UnlabelledOp?";
}

OM_END

//EOF
