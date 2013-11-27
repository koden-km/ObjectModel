// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Storage.h"
#include "Om/Class.h"

#include <boost/unordered_map.hpp>

OM_BEGIN

struct RegistryImpl;

/// a registry is a class-factory, storing types and instances
struct Registry
{
	//typedef std::map<Handle, StorageBase *> Instances;
	//typedef std::map<Type::Number, ClassBase *> Classes;
//	boost::scoped_ptr<RegistryImpl> impl;
	typedef std::vector<Handle> Deathrow;
	typedef boost::unordered_map<Handle::Value, StorageBase *> Instances;
	typedef boost::unordered_map<Type::Number::Value, ClassBase *> Classes;

private:

	Instances instances;
	Classes classes;
	Handle::Value next_handle;
	Object root;
	Deathrow deathrow;

public:
	Registry();
	~Registry();

	void Clear();
	void SetRoot(Object);
	Object GetRoot();

	void EnableTrace(bool = true);
	void GarbageCollect();

	size_t NumObjects() const { return instances.size(); }
	void Delete(Object &);
	void Delete(Handle);

	ClassBase const *GetClass(Type::Number type_number) const;
	ClassBase const *GetClassFromLabel(Label const &) const;
	bool HasClass(Label const &L) const { return GetClassFromLabel(L) != 0; }
	bool HasClass(Type::Number type_number) const { return classes.find(type_number.value) != classes.end(); }
	bool HasObject(Handle handle) const { return instances.find(handle.value) != instances.end(); }
	Object NewFromClassName(Label const &);
	Object NewFromTypeNumber(Type::Number);
	Object NewFromClass(ClassBase const *);

	StorageBase &GetStorage(Handle);
	Object GetObject(Handle) const;
	bool ObjectExists(Handle) const;

	template <class T>
	Class<T> *AddClass()
	{
		Type::Number type_number = Type::Traits<T>::TypeNumber;
		if (HasClass(type_number))
		{
			return GetClass<T>();//type_number);//OM_THROW_0(ClassAlreadyRegistered);
		}
		Class<T> *klass = new Class<T>(*this, type_number);
		classes[type_number.value] = klass;
		return klass;
	}

	template <class T>
	Object New()
	{
		Class<T> *klass = GetClass<T>();
		ClassBase *base = reinterpret_cast<ClassBase *>(klass);
		return NewFromClass(base);
	}

	template <class T>
	Object New(T const &X)
	{
		Pointer<T> P = New<T>();
		Type::Traits<T>::Assignment()(*P, X);
		return P;
	}

	Object New(Object const &Q) const
	{
		return Q;
	}

	template <class T>
	Object Ref(T &X)
	{
		Class<T> const *klass = static_cast<Class<T> const *>(GetClass<T>());//(Type::Traits<T>::TypeNumber));//
		Handle handle(++next_handle);
		Reference<T> *R = klass->NewReference(handle, X);
		instances[handle.value] = R;
		return *R;
	}
	template <class U>
	Object ConstRef(U const &X)
	{
		typedef typename BaseType<U>::Type T;
		Class<T> const *klass = static_cast<Class<T> const *>(GetClass<T>());//(Type::Traits<T>::TypeNumber));
		Handle handle(++next_handle);
		ConstReference<T> *R = klass->NewConstReference(handle, X);
		instances[handle.value] = R;
		return *R;
	}
	ClassBase *GetClassFromLabel(Label const &);

private:
	template <class T>
	Class<T> *GetClass()
	{
		Classes::iterator k = classes.find(Type::Traits<T>::TypeNumber.value);
		if (k == classes.end())
			return AddClass<T>();
		return reinterpret_cast<Class<T> *>(k->second);
	}
	void Mark(Object const &, bool);
	void MarkAll(bool);
};

void RegisterBuiltins(Registry &registry);

OM_END

//EOF
