#pragma once

#include "Om/Debug/Trace.h"

namespace Debug
{
	//using Trace::ITraceable;

	void Write(ITraceable const &);
	void Write(IToString const &);
	void Write(const char *fmt, ...);
	void Write(String const &);

	template <class Stream, class A, class B>
	void Write(Stream &S, std::pair<A,B> const &P)
	{
		S << "pair(" << P.first << ", " << P.second << ")";
	}

	template <class Str, class T>
	Str &Write(Str &S, T const &X)
	{
		detail::Selector<T>::Write(S, X);
		return S;
	}

	template <class Str, class T>
	Stream<Str> &operator<<(Stream<Str> &S, T const &X)
	{
		Write(S, X);
		return S;
	}

	template <class T>
	String ToString(T const &X)
	{
		Debug::StringStream S;
		Write(S, X);
		return S.ToString();
	}
	template <class Str>
	Str &Write(Str &S, bool B)
	{
		return S << (B ? "true" : "false");
	}
}

//EOF
