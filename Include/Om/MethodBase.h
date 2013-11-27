// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// common for all Method<..>'s
struct MethodBase : HasLabel//, IToString
{
	Type::Signature signature;
	virtual void Invoke(Object &servant, Vector &args) const = 0;

	String ToString(Registry &R) const;
};

OM_TRAITS_NUMBER_NAME_OPS(MethodBase const *, Number::Method, "Method", Operations::Assign);

OM_END

//EOF
