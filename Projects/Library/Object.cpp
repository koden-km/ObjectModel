// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Registry.h"

OM_BEGIN

const Object null;

Object::Object() 
	: registry(0), klass(0)
{
}

Object::Object(Registry *R, Handle H)
	: registry(R), handle(H)
{
	if (registry == 0)
	{
		OM_THROW(NullObject);
	}
	klass = registry->GetClass(registry->GetObject(handle).GetTypeNumber());
}

Object::Object(Registry *R, ClassBase const *C, Handle H)
	: registry(R), klass(C), handle(H)
{
}

Object::Object(Object const &Q)
	: registry(Q.registry), handle(Q.handle), klass(Q.klass)
{
}

Label const &Object::GetName() const
{
	return GetStorage().HasLabel::GetLabel();
}

String Object::GetIdent() const
{
	StringStream S;
	S << GetName().ToString().c_str() << "#" << GetHandle().value;
	return S.str();
}


bool Object::IsReference() const
{
	return GetStorage().IsReference();
}

bool Object::IsConst() const
{
	return GetStorage().IsConst();
}

void Object::Delete()
{
	if (IsEmpty())
		return;
	registry->Delete(*this);
}

void Object::SetMarked(MarkingContext &mark) const
{
	if (!Exists())
		return;
	GetStorage().SetMarked(mark);
}

bool Object::IsMarked() const
{
	return GetStorage().IsMarked();
}

Object Object::Clone() const
{
	if (!Exists())
		return Object();
	Object clone = klass->Create();
	klass->Assign(clone, *this);
	return clone;
}

bool Object::IsEmpty() const
{
	bool empty = registry == 0 || klass == 0 || handle == 0;// || IsType<None>();
	return empty || !registry->HasObject(handle);
}

Type::Number Object::GetTypeNumber() const 
{ 
	return klass ? klass->GetTypeNumber() : Type::Number::None; 
}

ObjectProxy Object::operator[](Label const &label)
{
	return ObjectProxy(*this, label);
}

ObjectProxy Object::operator[](const char *label)
{
	return ObjectProxy(*this, label);
}

StorageBase &Object::GetStorage()
{
	if (IsEmpty())
	{
		OM_THROW_0(NullObject);
	}
 	return registry->GetStorage(handle);
}

StorageBase const &Object::GetStorage() const
{
	if (IsEmpty())
	{
		OM_THROW_0(NullObject);
	}
	return registry->GetStorage(handle);
}

Registry &Object::GetRegistry() const
{
	if (!registry)
	{
		OM_THROW_0(NullObject);
	}
	return *registry;
}

String Object::ToString() const
{
	if (!Exists())
		return "null";
	Debug::StringStream S;
	if (GetClass()->HasOperation(Type::Operations::StringInsert))
		GetClass()->Trace(S, *this);
	else
		S << "Instance of " << GetClass()->GetName().ToString() << " at @" << &GetStorage();
	return S.ToString();
}

void Object::Trace(Debug::StringStream &S) const
{
	if (!Exists())
	{
		S << "null";
		return;
	}
	if (!klass->HasOperation(Type::Operations::StringInsert))
	{
		S << "instance of " << klass->GetName().GetText().c_str() << "@" << &GetStorage() << endl;
	}
	else
	{
		String type = GetClass()->GetName().ToString();
		S << "{" << type << ": ";
		GetClass()->Trace(S, *this);
		S << "}";
	}
}

bool Object::HasMethod(Label const &L) const
{
	return Exists() && GetClass()->HasMethod(L);
}

ClassBase const *Object::GetClassBase() const
{
	if (!klass)
		OM_THROW(NullObject);
	return klass;
}

MethodBase const *Object::GetMethod(Label const &L) const
{
	return GetClassBase()->GetMethod(L);
}

void Object::Set(Label const &label, Object &object)
{
	GetStorage().Set(label, object);
}

Object Object::Get(Label const &label) const
{
	return GetStorage().Get(label);
}

Object Object::Get(Label const &label)
{
	return GetStorage().Get(label);
}

bool Object::Has(Label const &label) const
{
	return Get(label).Exists();
}

void Object::Remove(Label const &label)
{
	return GetStorage().Remove(label);
}

Object Object::GetParent() const
{
	return GetStorage().GetParent();
}

Handle Object::GetParentHandle() const
{
	return GetStorage().GetParentHandle();
}

void Object::Attach(Object const &parent, Label const &label)
{
	GetStorage().Attach(parent, label);
}

void Object::Detach(Object const &parent)
{
	GetStorage().Detach(parent);
}

PropertyBase const *Object::GetPropertyObject(Label const &label) const
{
	return GetClass()->GetProperty(label);
}

void AddDeepCopy(Object &parent, Label const &label, Object const &child)
{
	if (!child)
		return;
	parent.Set(label, DeepCopy(child));
}

Object DeepCopy(Object const &object)
{
	if (!object)
		return Object();
	Object clone = object.GetClass()->Create();
	Assign(clone, object);
	object.GetStorage().ForEachSubObject(boost::bind(AddDeepCopy, boost::ref(clone), _1, _2));
	return clone;
}

OM_END

//EOF
