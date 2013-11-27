// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// heap-based storage for a given object type
template <class T>
struct Storage : StorageBase
{
	typedef typename Type::Traits<T>::Storage MyStorage;
	typedef typename Type::Traits<T>::Reference Reference;
	typedef typename Type::Traits<T>::ConstReference ConstReference;
	MyStorage storage;

	Storage(Object const &Q) : StorageBase(Q) { }
	Storage(Object const &Q, ConstReference X) : StorageBase(Q), storage(X)  { }

	Reference GetReference() { return storage; }
	ConstReference GetConstReference() const { return storage; }
};

/// storage for a reference to an existing, external object value
template <class T>
struct Reference : StorageBase
{
	typedef typename Type::Traits<T>::Storage MyStorage;
	typedef typename Type::Traits<T>::Reference ReferenceType;
	typedef typename Type::Traits<T>::ConstReference ConstReference;
	ReferenceType ref;

	Reference(ReferenceType R, Object const &Q) : StorageBase(Q, true, false), ref(R) { }

	ReferenceType GetReference() { return ref; }
	ConstReference GetConstReference() const { return ref; }
};

/// storage for a const reference to an existing, external object value
template <class T>
struct ConstReference : StorageBase
{
	typedef typename Type::Traits<T>::Reference Reference;
	typedef typename Type::Traits<T>::ConstReference ConstReferenceType;
	ConstReferenceType ref;

	ConstReference(ConstReferenceType R, Object const &Q) : StorageBase(Q, true, true), ref(R) { }

	Reference GetReference() { OM_THROW(ConstError); }
	ConstReference GetConstReference() const { return ref; }
};

OM_END

//EOF
