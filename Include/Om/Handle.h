// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

template <class T>
struct NumericValue : Debug::ITraceable
{
	typedef T Value;
	Value value;

	NumericValue(Value H = 0) : value(H) { }

	friend bool operator==(NumericValue A, NumericValue B) { return A.value == B.value; }
	friend bool operator!=(NumericValue A, NumericValue B) { return A.value != B.value; }
	friend bool operator<(NumericValue A, NumericValue B) { return A.value < B.value; }

	void Trace(Debug::StringStream &S) const
	{
		S << value;
	}
};

/// the numeric identifier of an object
struct Handle : NumericValue<unsigned short>
{
	Handle(Value H = 0) : NumericValue<Value>(H) { }
};

OM_END

//EOF
