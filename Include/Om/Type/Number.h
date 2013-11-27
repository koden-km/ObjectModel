// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

namespace Type
{
	/// type nubers used for builtin types
	struct Number : NumericValue<unsigned short>
	{
		Number(Value H = 0) : NumericValue<Value>(H) { }

		enum Type
		{
			None,
			Void,
			TypeNumber,
			Object,
			Continuation,
			Lambda,
			Compiler,
			Executor,

			Bin,
			Label,
			Pathname,
			Signature,
			Method,
			Function,
			Class,
			Operation,

			String,
			StringStreamT,
			BinaryStream,

			Bool,

			Int,
			Size,
			Char,

			Float,
			Float2,
			Float3,
			Float4,
			Float4x4,
			WideString,
			Vector,
			Map,
			List,
			Set,

			LastBuiltin = 100
		};
	};

} // namespace Type

#define OM_USER_TYPE_NUMBE(N) \
	(OM_ROOT_NAMESPACE(Type::Number::LastBuiltin) + N)

OM_END

//EOF
