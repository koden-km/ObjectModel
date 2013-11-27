// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/BinaryStream.h"

OM_BEGIN

/// a system class for a given type T
///
/// the class provides overrides for the ClassBase virtual methods, providing access to type-specific functionality
template <class T>
struct Class : ClassBase
{
	typedef Type::Traits<T> Traits;		///< the traits for the type
	typedef typename Traits::Storage Value;

	Class(Registry &R, Type::Number N) 
		: ClassBase(R, N, Traits::TypeName)
	{
	}

	Object Create() const
	{
		return registry->New<T>();
	}

	void Destroy(Object &Q) const
	{
		Q.GetStorage().konst = false;
		Traits::ReflectedOps::Destroy(Deref<T>(Q));
	}

	StorageBase *NewStorage(Handle handle) const
	{
		Storage<T> *storage = new Storage<T>(Object(registry, this, handle));
		return storage;
	}

	void SetProperties(Object &object) const
	{
		foreach (Properties::value_type const &iter, GetProperties())
		{
			PropertyBase const *prop = iter.second;
			(const_cast<PropertyBase *>(prop))->SetOwner(object, prop->GetLabel());
		}
		Storage<T> *storage = static_cast<Storage<T> *>(&object.GetStorage());
		Traits::ReflectedOps::Create(storage->GetReference(), *storage);
	}
	Reference<T> *NewReference(Handle handle, Value &R) const
	{
		return new Reference<T>(R, Object(registry, this, handle));
	}
	ConstReference<T> *NewConstReference(Handle handle, Value const &R) const
	{
		return new ConstReference<T>(R, Object(registry, this, handle));
	}

	void DeleteStorage(StorageBase *base) const
	{
		if (base->IsReference())
		{
			if (base->IsConst())
				delete static_cast<ConstReference<T> *>(base);
			else 
				delete static_cast<Reference<T> *>(base);
		}
		else
			delete static_cast<Storage<T> *>(base);
	}

	void MarkContained(Object const &object, MarkingContext &mark) const
	{
		Traits::Mark()(Deref<T>(object), mark);
	}

	bool HasOperation(Type::Operations::Type op) const
	{
		return (Traits::Operations & op) != 0;
	}

	void Assign(Object &A, Object const &B) const
	{
		Traits::Assignment()(Deref<T>(A), Deref<T>(B));
	}

	template <class Op>
	Object BinaryOp(Op op, Object const &A, Object const &B) const
	{
		return registry->New(op(Deref<T>(A), Deref<T>(B)));
	}
	template <class Op>
	bool RelationalOp(Op op, Object const &A, Object const &B) const
	{
			return op(Deref<T>(A), Deref<T>(B));
	}
	template <class Op>
	bool RelationalOp(Op op, Object const &A) const
	{
		return op(Deref<T>(A));
	}

	Object Plus(Object const &A, Object const &B) const
	{
		return BinaryOp(Traits::Plus(), A, B);
	}
	Object Minus(Object const &A, Object const &B) const
	{
		return BinaryOp(Traits::Minus(), A, B);
	}
	Object Multiply(Object const &A, Object const &B) const
	{
		return BinaryOp(Traits::Multiply(), A, B);
	}
	Object Divide(Object const &A, Object const &B) const
	{
		return BinaryOp(Traits::Divide(), A, B);
	}

	bool Less(Object const &A, Object const &B) const
	{
		return RelationalOp(Traits::Less(), A, B);
	}
	bool Equiv(Object const &A, Object const &B) const
	{
		return RelationalOp(Traits::Equiv(), A, B);
	}

	bool Boolean(Object const &A) const
	{
		return RelationalOp(Traits::Bool(), A);
	}
	bool Not(Object const &A) const
	{
		return RelationalOp(Traits::Not(), A);
	}

	void Trace(Debug::StringStream &S, Object const &X) const
	{
		Traits::Tracer()(S,Deref<T>(X));
	}

	void Write(BinaryStream &S, Object const &Q) const
	{
		Traits::BinaryWrite()(S, Deref<T>(Q));
	}
	void Read(BinaryStream &S, Object &Q) const
	{
		Q = Create();
		Traits::BinaryRead()(S, Deref<T>(Q));
	}

	void WriteContents(BinaryStream &S, Object const &Q) const
	{
		// TODO
	}
	void ReadContents(BinaryStream &S, Object &Q) const
	{
		// TODO
	}
};

OM_END

//EOF
