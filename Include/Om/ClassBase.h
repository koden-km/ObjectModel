// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <map>

OM_BEGIN

template <class T>
struct Class;

/// structure common to all class types. forwards requests to the derived, specialised class<T> via pure virtual calls
struct ClassBase : Debug::ITraceable
{
	typedef std::map<Label, MethodBase const *> Methods;
	typedef std::map<Label, PropertyBase const *> Properties;

protected:
	Registry *registry;
	Methods methods;
	Properties properties;
	Type::Number type_number;
	Label name;

public:
	ClassBase(Registry &R, Type::Number N, const char *K) : registry(&R), type_number(N), name(K) { }

	void AddProperty(Label const &label, PropertyBase *prop);
	void AddMethod(Label const &label, MethodBase *method);

	void Trace(Debug::StringStream &) const;
	bool HasProperty(Label const &L) const { return GetProperty(L) != 0; }
	bool HasMethod(Label const &L) const { return GetMethod(L) != 0; }

	Methods const &GetMethods() const { return methods; }
	Properties const &GetProperties() const { return properties; }

	MethodBase const *GetMethod(Label const &) const;
	PropertyBase const *GetProperty(Label const &) const;
	Type::Number GetTypeNumber() const { return type_number; }
	Label const &GetName() const { return name; }


	//void Mark(Object const &object, bool mark);

	virtual void MarkContained(Object const &object, MarkingContext & mark) const = 0;
	virtual void Trace(Debug::StringStream &, Object const &) const = 0;
	virtual void SetProperties(Object &object) const = 0;

	virtual StorageBase *NewStorage(Handle) const = 0;
	virtual void DeleteStorage(StorageBase *) const = 0;

	virtual Object Create() const = 0;
	virtual void Destroy(Object &) const = 0;
	virtual void Assign(Object &, Object const &) const = 0;

	virtual bool HasOperation(Type::Operations::Type op) const = 0;

	virtual bool Boolean(Object const &) const = 0;
	virtual bool Not(Object const &) const = 0;

	virtual Object Plus(Object const &, Object const &) const = 0;
	virtual Object Minus(Object const &, Object const &) const = 0;
	virtual Object Multiply(Object const &, Object const &) const = 0;
	virtual Object Divide(Object const &, Object const &) const = 0;

	virtual bool Equiv(Object const &, Object const &) const = 0;
	virtual bool Less(Object const &, Object const &) const = 0;

	virtual void Write(BinaryStream &, Object const &) const = 0;
	virtual void Read(BinaryStream &, Object &) const = 0;
	virtual void WriteContents(BinaryStream &, Object const &) const = 0;
	virtual void ReadContents(BinaryStream &, Object &) const = 0;

	bool NotEquiv(Object const &A, Object const &B) const
	{
		return !Equiv(A,B);
	}
};

template <class Str>
void Write(Str &S, OM_ROOT_NAMESPACE(ClassBase) const *klass)
{
	if (!klass) 
		S << "null class";
	else
		klass->Trace(S);
}

OM_TRAITS_NUMBER_NAME_OPS(ClassBase const *, Number::Class, "Class", Operations::StringInsert | Operations::Assign);

OM_END

namespace Debug
{
	using Om::Write;
}

//EOF
