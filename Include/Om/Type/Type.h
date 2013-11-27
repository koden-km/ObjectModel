// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

namespace Type
{
	struct Type : Modifier, Debug::ITraceable
	{
		Number number;

		Type(Number N = Number::None, Modifier M = Modifier()) : Modifier(M), number(N) { }

		Number GetNumber() const { return number; }

		void Trace(Debug::StringStream &) const;
		String ToString(Registry &) const;
	};

	template <class T>
	struct is_const
	{
		BOOST_STATIC_CONSTANT(bool, value = false);
	};
	template <class T>
	struct is_const<const T>
	{
		BOOST_STATIC_CONSTANT(bool, value = true);
	};
	template <class T>
	struct is_const<const T&>
	{
		BOOST_STATIC_CONSTANT(bool, value = true);
	};

	template <class T>
	struct MakeType
	{
		static Type Create()
		{
			Modifier modifier;
			modifier.SetConst(is_const<T>::value);	// WTF is wrong with boost::is_const<T> ??
			modifier.SetReference(boost::is_reference<T>::value);
			return Type(Traits<typename BaseType<T>::Type>::TypeNumber, modifier);
		}
	};
}

OM_END

//EOF
