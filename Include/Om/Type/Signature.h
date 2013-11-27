// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

namespace Type
{
	/// a method signature
	struct Signature
	{
		Type class_type;
		Type return_type;
		bool konst;
		std::vector<Type> arguments;

		String ToString(Registry &);
	};
}

OM_TRAITS(Signature);

OM_END

//EOF
