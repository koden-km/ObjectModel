// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

namespace Type
{
	/// a type modifier: const and/or reference
	struct Modifier
	{
		typedef std::bitset<3> Flags;
		enum Type
		{
			None = 0,
			Const = 1,
			Reference = 2,
		};
		Flags flags;

		Modifier() : flags((_ULonglong)0) { }
		Modifier(int N) : flags(N) { }

		bool IsConst() const { return flags[Const]; }
		bool IsReference() const { return flags[Reference]; }
		void SetConst(bool B = true) { flags[Const] = B; }
		void SetReference(bool B = true) { flags[Reference] = B; } 
		void SetConstReference(bool B = true) { SetConst(B); SetReference(B); }
	};
}

OM_END

//EOF
