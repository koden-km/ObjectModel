// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// used for operator[]
struct ObjectProxy;

/// a generic system object
struct Object : Debug::ITraceable, IMarkable, IToString
{
protected:
	Registry *registry;			///< the registry that made this object
	ClassBase const *klass;		///< the object class
	Handle handle;				///< the integral handle that specifies this object in the registry

public:
	Object();
	Object(Registry *, Handle);
	Object(Registry *, ClassBase const *, Handle);
	Object(Object const &);

	String ToString() const;
	void Delete();
	Label const &GetName() const;
	String GetIdent() const;
	void SetMarked(MarkingContext &mark) const;
	bool IsMarked() const;
	template <class Fun>
	void ForEachChild(Fun &fun) const
	{
		GetStorage().ForEachChild(fun);
	}
	template <class Fun>
	void ForEachChild(Fun &fun)
	{
		GetStorage().ForEachChild(fun);
	}
	/// return true if this object doesn't exist, or is of type None
	bool IsEmpty() const;
	bool IsReference() const;// { return reference; }
	bool IsConst() const;// { return konst; }

	bool Exists() const { return !IsEmpty(); }
	
	Type::Number GetTypeNumber() const;
	Registry &GetRegistry() const;
	ClassBase const *GetClassBase() const;
	ClassBase const *GetClass() const { return GetClassBase(); }

	bool HasMethod(Label const &) const;
	MethodBase const *GetMethod(Label const &) const;

	Handle GetHandle() const { return handle; }
	Object GetParent() const;
	Handle GetParentHandle() const;
	bool HasParent() const { return GetParent().Exists(); }

	operator bool() const { return Exists(); }

	template <class T>
	Object New() const
	{
		if (!registry)
			return Object();
		return registry->New<T>();
	}

	template <class T>
	Object New(T const &X) const
	{
		if (!registry)
			return Object();
		return registry->New(X);
	}
	Object New(Object const &Q) const
	{
		return Q;
	}

	template <class T>
	Object Ref(T &X) const
	{
		if (!registry)
			return Object();
		return registry->Ref<T>(X);
	}
	template <class T>
	Object ConstRef(T &X) const
	{
		if (!registry)
			return Object();
		return registry->ConstRef<T>(X);
	}
	Object Clone() const;

	template <class T>
	bool IsType() const { return GetTypeNumber() == Type::Traits<T>::TypeNumber; }

	StorageBase &GetStorage();
	StorageBase const &GetStorage() const;

	void Trace(Debug::StringStream &) const;

	void Set(Label const &, Object &);
	Object Get(Label const &) const;
	Object Get(Label const &);
	bool Has(Label const &) const;
	void Remove(Label const &);

	ObjectProxy operator[](Label const &label);
	ObjectProxy operator[](const char *);

	template <class T>
	void SetValue(Label const &label, T const &X)
	{
		Object child = Get(label);
		Deref<typename BaseType<T>::Type>(child) = X;
	}

	template <class T>
	T const &GetValue(Label const &label) const
	{
		Object child = Get(label);
		return Deref<typename BaseType<T>::Type>(child);
	}

//protected:
	PropertyBase const *GetPropertyObject(Label const &label) const;
	void Attach(Object const &parent, Label const &);
	void Detach(Object const &parent);
};

OM_TRAITS_NUMBER_NAME_OPS(Object, Number::Object, "Object", Operations::Streaming);

struct ObjectProxy
{
	Object object;
	Label const label;
	ObjectProxy(Object const &Q, Label const &L) : object(Q), label(L) { }
	ObjectProxy& operator=(Object const &child)
	{
		Object ref(child);
		object.Set(label, ref);
		return *this;
	}
	ObjectProxy& operator=(Object &child)
	{
		object.Set(label, child);
		return *this;
	}
	Object Get() const
	{
		return object.Get(label);
	}
	operator Object()
	{
		return Get();
	}
	operator Object() const
	{
		return Get();
	}
	ObjectProxy operator[](Label const &label)
	{
		return Get()[label];
	}
};

extern const Object null;

void Assign(Object &, Object const &);
Object Plus(Object const &, Object const &);
Object Minus(Object const &, Object const &);
Object Divide(Object const &, Object const &);
Object Multiply(Object const &, Object const &);
Object Dereference(Object const &);
bool Boolean(Object const &);
bool Not(Object const &);
bool Equiv(Object const &, Object const &);
bool Less(Object const &, Object const &);

/// makes a new object from an existing one, and copies the value
Object Duplicate(Object const &);

/// makes a new object from an existing one, and copies the value, and does the same recursively for all sub-object
Object DeepCopy(Object const &Q);

OM_END

//EOF
