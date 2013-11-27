// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Registry.h"

OM_BEGIN

StorageBase::StorageBase(Object const &Q, bool r, bool k) 
	: Object(Q), reference(r), konst(k)
{
}

StorageBase::~StorageBase()
{
	Object parent = GetParent();
	if (parent)
	{
		parent.Remove(GetLabel());
	}
	ForEachChild(bind(&StorageBase::Detach, _1, boost::cref(*this)));
}

bool StorageBase::HasParent() const
{
	return Exists() && registry->HasObject(parent_handle);
}

Object StorageBase::GetParent() const
{
	if (!Exists())
		return Object();
	return registry->GetObject(parent_handle);
}

void StorageBase::Attach(Object const &new_parent, Label const &label)
{
	Object parent = registry->GetObject(parent_handle);
	if (parent)
		Detach(parent);
	SetLabel(label);
	parent_handle = new_parent.GetHandle();
}

void StorageBase::Detach(Object const &parent)
{
	if (parent.GetHandle() != parent_handle)
	{
		OM_TRACE_3(0, *this, parent, registry->GetObject(parent_handle));
		assert(parent.GetHandle() == parent_handle);
	}
	parent_handle = 0;
	SetLabel(Label());
}

bool StorageBase::Has(Label const &label) const
{
	return Get(label).Exists();
}

void StorageBase::Set(Label const &label, Object &child)
{
	// remove existing child object
	Remove(label);

	// check for property first
	PropertyBase const *prop = GetPropertyObject(label);
	if (prop)
	{
		prop->Set(*this, child);
		if (child && prop->IsObject())
			child.Attach(*this, label);
		return;
	}

	// store to dictionary if not a property
	dictionary[label] = child;
	if (!child)
		return;
	child.Attach(*this, label);
}

void StorageBase::Remove(Label const &label)
{
	PropertyBase const *prop = GetPropertyObject(label);
	if (prop)
	{
		Object child = prop->Get(*this);
		if (!child)
			return;
		if (child.GetParentHandle() == handle)
			child.Detach(*this);
		prop->Reset(*this);
		return;
	}
	Object child = dictionary.Find(label);
	if (!child)
		return;
	dictionary.Erase(label);
	child.Detach(*this);
}

Object StorageBase::Get(Label const &label) const
{
	PropertyBase const *prop = GetPropertyObject(label);
	if (prop)
		return prop->Get(*this);
	return dictionary.Find(label);
}

Object StorageBase::Get(Label const &label)
{
	PropertyBase const *prop = GetPropertyObject(label);
	if (prop)
		return prop->Get(*this);
	return dictionary.Find(label);
}

void StorageBase::SetMarked(MarkingContext &mark) const
{
	if (!mark.Enter(GetHandle()))
		return;
	marked = mark.Mark();
	ForEachChild(boost::bind(&StorageBase::SetMarked, _1, ref(mark)));
	klass->MarkContained(*this, mark);
}

bool StorageBase::IsMarked() const
{
	return marked;
}

OM_END

//EOF
