#pragma once

namespace Debug
{
	namespace detail
	{
		template <class>
		struct Unwrap
		{
			template <class Str>
			static Str& Get(Str& S) { return S; }
		};

		template <class Str>
		struct Unwrap<Stream<Str> >
		{
			static Str& Get(Stream<Str>& S) { return S.stream; }
		};

		template <bool traceable, bool tostring>
		struct SelectWriter
		{
			template <class Str, class T>
			static void Write(Str &S, T const &X)
			{
				Unwrap<Str>::Get(S) << X;
			}
		};

		template <bool B>
		struct SelectWriter<true, B>
		{
			template <class Str, class T>
			static void Write(Str &S, T const &X)
			{
				X.Trace(S);
			}
		};
		template <bool B>
		struct SelectWriter<B, true>
		{
			template <class Str, class T>
			static void Write(Str &S, T const &X)
			{
				Unwrap<Str>::Get(S) << X.ToString();
			}
		};
		template <>
		struct SelectWriter<true, true>
		{
			template <class Str, class T>
			static void Write(Str &S, T const &X)
			{
				X.Trace(S);
			}
		};
		template <class T>
		struct Selector 
			: SelectWriter<boost::is_convertible<T *, ITraceable *>::value, boost::is_convertible<T *, IToString *>::value>
		{
		};

	} // namespace detail

} // namespace Debug

//EOF
