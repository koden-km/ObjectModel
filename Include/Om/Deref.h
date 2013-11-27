// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// implemenation for the dereference operation, specialised for different argument types
template <class T>
struct DerefImpl
{
	typedef Type::Traits<T> Traits;
	typedef typename Traits::Reference Reference;
	typedef typename Traits::ConstReference ConstReference;

	static Reference Deref(Object &Q)
	{
		StorageBase &base = Q.GetStorage();
		if (!base.IsType<T>())
		{
			OM_THROW_0(TypeMismatch);
		}
		if (base.IsConst())
		{
			OM_THROW(ConstError);
		}
		if (base.IsReference())
		{
			return static_cast<OM_ROOT_NAMESPACE(Reference)<T> &>(base).GetReference();
		}
		return static_cast<Storage<T> &>(base).GetReference();
	}

	static ConstReference ConstDeref(Object const &Q)
	{
		StorageBase const &base = Q.GetStorage();
		if (!base.IsType<T>())
		{
			OM_THROW_0(TypeMismatch);
		}
		if (base.IsReference())
		{
			return static_cast<OM_ROOT_NAMESPACE(Reference)<T> const &>(base).GetConstReference();
		}
		return static_cast<Storage<T> const &>(base).GetConstReference();
	}
};

/// special case for dereferencing an untyped object; just return the given object
template <>
struct DerefImpl<Object>
{
	typedef Object &Reference;
	typedef Object const &ConstReference;
	static Reference Deref(Object &Q)
	{
		return Q;
	}
	static ConstReference ConstDeref(Object const &Q)
	{
		return Q;
	}
};

template <class T>
typename DerefImpl<T>::Reference Deref(ObjectProxy const &Q)
{
	return DerefImpl<T>::Deref(Q.Get());
}

template <class T>
typename DerefImpl<T>::Reference Deref(Object &Q)
{
	return DerefImpl<T>::Deref(Q);
}

template <class T>
typename DerefImpl<T>::ConstReference Deref(Object const &Q)
{
	return DerefImpl<T>::ConstDeref(Q);
}

template <class T>
typename DerefImpl<T>::ConstReference ConstDeref(Object const &Q)
{
	return DerefImpl<T>::ConstDeref(Q);
}

OM_END

//EOF
