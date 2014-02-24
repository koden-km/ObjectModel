#pragma once

namespace Debug
{
	/// a structure that prints a stream when it leaves scope
	/// can be used as an initial stream sink:
	///
	/// Printer<Trace::StringStream> ptr(..) << args << ...;
	///
//		template <class Str>
	struct Printer
	{
		//Stream<Str> &stream;
		StringStream &input;
		StringStream total;

		Printer(StringStream &input, FileLocation const &);
		~Printer();
	};

	template </*class Str, */class T>
	StringStream& operator<<(Printer/*<Str> */&S, T const &X)
	{
		return S.total << X;
	}

	/// returns the stream at the given index.
	/// these streams can be used for whatever purpose.
	StringStream &GetStream(size_t);

	void EnableStream(size_t, bool);
}

//EOF
