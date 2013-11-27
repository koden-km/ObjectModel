// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// a field to be used to expose a member object to the system
template <class T>
struct Field : Pointer<T>, HasLabel, boost::noncopyable
{
	Object owner;

	Field() 
	{
	}

	// when copying a field, do not change the owner
	Field(Field const &Q)
	{ 
		Pointer<T>::operator=(Q);
	}

	// when copying a field, do not change the owner
	Field &operator=(Field const &Q)
	{
		Pointer<T>::operator=(Q);
		return *this;
	}

	Field& operator=(Object &Q)
	{
		Pointer<T>::operator=(Q);
		owner.Set(GetLabel(), Q);
		return *this;
	}
	//Field& operator=(T const &X)
	//{
	//	owner.SetValue(GetLabel(), X);
	//	return *this;
	//}

	void SetOwner(Object const &Q, Label const &L)
	{
		SetLabel(L);
		owner = Q;
	}
};

OM_END

//EOF
