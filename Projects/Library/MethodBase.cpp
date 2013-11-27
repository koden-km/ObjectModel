// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/MethodBase.h"
#include "Om/Registry.h"
#include "Om/Debug/Trace/Containers.h"

OM_BEGIN

namespace Type
{
	void Type::Trace(Debug::StringStream &S) const
	{
		OM_TRACE_OBJECT_2("Type", S, number.value, flags);
	}
	String Type::ToString(Registry &R) const
	{
		StringStream S;
		ClassBase const *klass = R.GetClass(number);
		if (klass == 0)
			S << "[Num: " << number.value << "]";
		else
			S << klass->GetName().ToString();
		if (IsConst())
			S << " const";
		if (IsReference())
			S << " &";
		return S.str();
	}

}

String MethodBase::ToString(Registry &R) const
{
	Debug::StringStream S;
	S << signature.return_type.ToString(R) << " " << GetLabel().ToString() << "(";
	foreach_delim_(Type::Type const &type, signature.arguments, ", ", S << type.ToString(R))
	S << ")";
	if (signature.konst)
		S << " const";
	S << ";";
	return S.ToString();
}

OM_END

//EOF
