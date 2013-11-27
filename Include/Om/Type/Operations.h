// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

#define OM_BIT(N) (1<<N)

namespace Type
{
	struct Operations
	{
		enum Type
		{
			Plus = OM_BIT(0),
			Minus = OM_BIT(1),
			Divide = OM_BIT(2),
			Multiply = OM_BIT(3),
			
			StringInsert = OM_BIT(4),
			StringExtract = OM_BIT(5),
			BinaryInsert = OM_BIT(6),
			BinaryExtract = OM_BIT(7),

			Dereference = OM_BIT(8),

			Bool = OM_BIT(9),
			Not = OM_BIT(10),

			Less = OM_BIT(11),
			Equiv = OM_BIT(12),
			Assign = OM_BIT(13),

			Arithmetic = Plus | Minus | Divide | Multiply,
			StringStreaming = StringInsert | StringExtract,
			BinaryStreaming = BinaryInsert | BinaryExtract,
			Streaming = StringStreaming | BinaryStreaming,
			Boolean = Bool | Not,
			Relational = Less | Equiv,

			None = 0,
			All = Arithmetic | Streaming | Relational | Boolean | Assign,
		};
	};

} // namespace Type

OM_END

//EOF
