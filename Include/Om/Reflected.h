// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// a reflected object has an internal Self pointer, which is analogous to C++'s 'this' pointer
/// it also has a strongly typed Typed pointer.
template <class U>
struct Reflected : ReflectedBase
{
	Storage<U> *Typed;

	// TODO: with some header re-ordering, these should be moved to ReflectedBase

	// these are just handy to allow members of reflected classes to use New<>, Ref<> and ConstRef<> nakedly
	template <class T>
	Object New() const
	{
		return Self->New<T>();
	}
	template <class T>
	Object New(T const &X) const
	{
		return Self->New<T>(X);
	}
	template <class T>
	Object Ref(T &X) const
	{
		return Self->Ref<T>(X);
	}
	template <class T>
	Object ConstRef(T const &X) const
	{
		return Self->ConstRef<T>(X);
	}
};

OM_END

//EOF
