#pragma once

namespace Debug
{
	template <class Str, class T>
	struct lazy_elem
	{
		T elem;
		lazy_elem(T const &X) : elem(X) { }
	};

	template <class Str>
	struct Stream : StreamBase
	{
		typedef Str StreamType;
		typedef Stream<Str> This;

	private:
		struct end_recurse { };
		Stream(end_recurse) { }

	public:
		StreamType stream;
		Stream()
		{
		}
		std::string ToString() const { return stream.str(); }
		void Clear() { stream.str(""); }	// hax

		void Write(char C) { stream << C; }
		void Write(char const *S) { stream << S; }
		void Write(std::string const &S) { Write(S.c_str()); }
	};

	template <class Str, class T>
	Stream<Str> &operator<<(Stream<Str> &S, lazy_elem<Str, T> const &X)
	{
		return S;
	}

	typedef Stream< ::StringStream> StringStream;

	template <class Str>
	Stream<Str> &operator<<(Stream<Str> &S, endl_struct)
	{
		S.NewLine();
		return S;
	}

	template <class Str>
	Stream<Str> &operator<<(Stream<Str> &S, indent_struct)
	{
		S.Indent();
		return S;
	}

	template <class Str>
	Stream<Str> &operator<<(Stream<Str> &S, outdent_struct)
	{
		S.Outdent();
		return S;
	}
	
	template <class Str>
	struct set_param_sep_struct
	{
		Stream<Str> &sep;
		set_param_sep_struct(Stream<Str> &S) : sep(S) { }
	};

	template <class Str>
	set_param_sep_struct<Str> set_param_sep(Stream<Str> &S)
	{
		return set_param_sep_struct<Str>(S);
	}

	template <class Str>
	Stream<Str> &operator<<(Stream<Str> &S, set_param_sep_struct<Str>(Stream<Str> &P))
	{
		return S;// TODO *S.ParamSep = P.sep;
	}

	template <class Str>
	Stream<Str> &operator<<(Stream<Str> &S, param_sep_struct)
	{
		return S << endl;//*S.ParamSep;
	}


	inline void test()
	{
		StringStream s;
		s << endl;
	}

} // namespace Debug

//EOF
