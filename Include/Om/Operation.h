// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// something that can be performed by an Executor
struct Operation : IToString
{
	enum Type
	{
		None,
		Trace,
		Typeof,
		New,
		Delete,
		Exists,
		This,
		Assert,

		Return,
		Break,
		Continue,
		DebugBreak,

		Assign,
		Equiv,
		NotEquiv,
		Less,
		LessEqual,
		Greater,
		GreaterEqual,

		LogicalOr,
		LogicalAnd,
		LogicalNot,
		LogicalTrue,	// (bool)Q

		Plus,
		Minus,
		Divide,
		Multiply,
		Modulo,
		PreInc,

		PlusAssign,
		MinusAssign,
		MultiplyAssign,
		DivideAssign,

		GetProperty,
		Suspend,
		Replace,
		Resume,
		MethodCall,
		FunctionCall,

		Drop,
		Swap,
		Dup,
		Clear,
		Quit,
		Rot,
		RotN,
		Roll,
		RollN,
		PickN,

		Enter,
		Leave,

		IfThenSuspend,
		IfThenReplace,
		IfThenResume,
		IfThenSuspendElseSuspend,
		IfThenReplaceElseSuspend,
		IfThenResumeElseSuspend,
		IfThenSuspendElseReplace,
		IfThenReplaceElseReplace,
		IfThenResumeElseReplace,
		IfThenSuspendElseResume,
		IfThenReplaceElseResume,
		IfThenResumeElseResume,

		Retrieve,
		Store,
		Name,

		ForLoop,
	};
	Type type;

	Operation() : type(None) { }
	Operation(Type T) : type(T) { }

	String ToString() const;
};

BinaryStream& operator<<(BinaryStream&, Operation const &);
BinaryStream& operator>>(BinaryStream&, Operation &);

OM_TRAITS_NUMBER_NAME_OPS(Operation, Number::Operation, "Operation", Operations::Streaming | Operations::Assign);

OM_END

//EOF
