// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Object.h"

OM_BEGIN

/// a strongly-typed object
template <class T>
struct Pointer : Object
{ 
	typedef typename Type::Traits<T> Traits;
	typedef typename Traits::Pointer Ptr;
	typedef typename Traits::ConstPointer ConstPointer;
	typedef typename Traits::Reference Reference;
	typedef typename Traits::ConstReference ConstReference;

	Pointer()
	{
	}

	Pointer(Object const &Q)
	{
		Assign(Q);
	}

	Pointer operator=(Object const &Q)
	{
		Assign(Q);
		return *this;
	}

	void Assign(Object const &Q)
	{
		if (!CanAssign(Q))
		{
			OM_THROW_0(TypeMismatch);
		}
		Object::operator=(Q);
	}

	bool CanAssign(Object const &Q)
	{
		if (Q.IsEmpty())
			return true;
		return Q.IsType<T>();
	}
	operator bool() const
	{
		return Exists();
	}

	Ptr operator->() { return &**this; }
	ConstPointer operator->() const{ return &**this; }

	Reference operator*() { return GetReference(); }
	ConstReference operator*() const{ return GetConstReference(); }

	Reference GetReference() { return Deref<T>(*this); }
	ConstReference GetConstReference() const { return Deref<T>(*this); }

};

/// pointer to const value
template <class T>
struct Pointer<const T> : Object
{
	// TODO
};

/// specialisation for the circular case of a pointer to an object
template <>
struct Pointer<Object> : Object
{
	Pointer() { }
	Pointer(Object const &Q) : Object(Q) { }

	Pointer operator=(Object const &Q)
	{
		Object::operator=(Q);
		return *this;
	}

	bool CanAssign(Object const &Q) { return true;  }
	operator bool() const { return Exists(); }

	Object *operator->() { return this; }
	Object const *operator->() const{ return this; }

	Object &operator*() { return *this; }
	Object const &operator*() const{ return *this; }
};

template <class T>
struct RemovePointer
{
	typedef T Type;
};

template <class T>
struct RemovePointer<Pointer<T> >
{
	typedef T Type;
};

/// remove any reference-ness or const-ness from a C++ type
template <class T>
struct BaseType
{
	typedef typename RemoveConstReference<T>::Type T0;
	typedef typename RemovePointer<T0>::Type Type;
};

OM_END

//EOF
