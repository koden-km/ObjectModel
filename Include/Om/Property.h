// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Pointer.h"
#include "Om/Field.h"

OM_BEGIN

/// a property with a value type
template <class Klass, class T>
struct Property : PropertyBase
{
	typedef T Klass::*Accessor;

	Accessor prop;

	Property(Accessor P, Label const L) : prop(P), PropertyBase(L) { }

	Object Get(Object &Q) const
	{
		return Q.Ref(Deref<Klass>(Q).*prop);
	}
	Object Get(Object const &Q) const
	{
		return Q.ConstRef(Deref<Klass>(Q).*prop);
	}

	void Set(Object &Q, Object &P) const
	{
		Deref<Klass>(Q).*prop = Deref<T>(P);
	}
	void Reset(Object &Q) const
	{
	}

	T &GetValue(Object &Q) const
	{
		return Deref<Klass>(Q).*prop;
	}
	void SetValue(Object &Q, T const &X) const
	{
		Deref<Klass>(Q).*prop = X;
	}

	bool IsObject() const
	{
		return false;
	}
};

/// a property with an Object, Pointer<> or Reference<> type
template <class Klass, class T>
struct Property<Klass, Field<T> > : PropertyBase
{
	typedef Field<T> Klass::*Accessor;

	Accessor prop;
	Property(Accessor P, Label const L) : prop(P), PropertyBase(L) { }

	Object Get(Object &Q) const
	{
		return GetField(Q);
	}
	Object Get(Object const &Q) const
	{
		return GetField(Q);
	}

	/// child management logic is done in StorageBase. do not call this directly
	void Set(Object &Q, Object &F) const
	{
		static_cast<Pointer<T> &>(GetField(Q)) = F;
	}
	void Reset(Object &Q) const
	{
		static_cast<Pointer<T> &>(GetField(Q)) = Object();
	}
	T &GetValue(Object &Q) const
	{
		return Deref<T>(GetField(Q));
	}
	void SetValue(Object &Q, T const &X) const
	{
		Deref<T>(GetField(Q)) = X;
	}
	bool IsObject() const
	{
		return true;
	}

	void SetOwner(Object &Q, Label const &L)
	{
		GetField(Q).SetOwner(Q, L);
	}

	Field<T> &GetField(Object &Q) const
	{
		return Deref<Klass>(Q).*prop;
	}
	Field<T> const &GetField(Object const &Q) const
	{
		return Deref<Klass>(Q).*prop;
	}
};

OM_END

//EOF
