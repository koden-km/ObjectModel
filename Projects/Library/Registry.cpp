// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Storage.h"
#include "Om/MethodBase.h"
#include "Om/Registry.h"
#include "Om/Deref.h"

OM_BEGIN

// the registry debug channel
static int rdc = 4;

Registry::Registry()
	: next_handle(0)
{
	EnableTrace(false);
}

Registry::~Registry()
{
	Clear();
}

void Registry::EnableTrace(bool E)
{
	OM_DEBUG_STREAM(rdc).Enable(E);
}

void Registry::Clear()
{
	root = Object();
	try
	{
		while (!instances.empty())
		{
			GarbageCollect();
		}
		foreach (Classes::value_type &val, classes)
		{
			delete val.second;
		}
	}
	catch (ExceptionBase &e)
	{
		OM_TRACE(3) << e;
	}

	instances.clear();
	classes.clear();
}

void Registry::GarbageCollect()
{
	// unmark all objects
	MarkAll(false);

	// mark everything that is reachable from the root
	Mark(root, true);

	// everything that is not now marked is not reachable and can be deleted
	deathrow.clear();
	foreach (Instances::value_type const &val, instances)
	{
		StorageBase const *base = val.second;
		if (!base->IsMarked())
		{
			//OM_TRACE_1(4, base->GetHandle().value) << " adding to deathrow";
			deathrow.push_back(base->GetHandle());
		}
	}

	// delete everything that is not reachable
	try
	{
		//OM_TRACE_1(0, deathrow.size());
		foreach (Handle handle, deathrow)
		{
			Delete(handle);
		}
	}
	catch (ExceptionBase &e)
	{
		OM_TRACE_1(3, e);
	}
	deathrow.clear();
}

void Registry::MarkAll(bool mark)
{
	MarkingContext context(mark);
	foreach (Instances::value_type &val, instances)
	{
		val.second->SetMarked(context);
	}
}

void Registry::Mark(Object const &object, bool mark)
{
	if (!object)
		return;
	MarkingContext context(mark);
	object.SetMarked(context);
}

void Registry::SetRoot(Object R)
{
	root = R;
}

void Registry::Delete(Object &Q)
{
	Delete(Q.GetHandle());
}

void Registry::Delete(Handle handle)
{
	OM_TRACE_1(4, handle.value) << " deleting";
	Instances::iterator iter = instances.find(handle.value);
	if (iter == instances.end())
	{
		OM_TRACE(4) << " ...not found";
		return;
	}
	StorageBase *base = iter->second;
	base->GetClass()->Destroy(*base);
	delete base;
	instances.erase(iter);
	OM_TRACE_1(4, handle.value) << " ...deleted";
}

bool Registry::ObjectExists(Handle handle) const
{
	return instances.find(handle.value) != instances.end();
}

Object Registry::GetRoot()
{
	if (!root)
	{
		root = New<void>();
	}
	return root;
}

StorageBase &Registry::GetStorage(Handle handle)
{
	Instances::iterator storage = instances.find(handle.value);
	if (storage == instances.end())
	{
		OM_THROW_0(NullObject);
	}
	return *storage->second;
}

Object Registry::GetObject(Handle handle) const
{
	Instances::const_iterator storage = instances.find(handle.value);
	if (storage == instances.end())
	{
		return Object();
	}
	return *storage->second;
}

ClassBase const *Registry::GetClass(Type::Number type_number) const
{
	Classes::const_iterator klass = classes.find(type_number.value);
	if (klass == classes.end())
		return 0;
	return klass->second;
}

Object Registry::NewFromClassName(Label const &label)
{
	ClassBase const *klass = GetClassFromLabel(label);
	if (klass == 0)
	{
		OM_THROW_1(UnknownClass, label);
	}
	return NewFromClass(klass);
}

ClassBase const *Registry::GetClassFromLabel(Label const &label) const
{
	foreach (Classes::value_type const &item, classes)
	{
		if (item.second->GetName().GetText() == label.GetText())
		{
			return item.second;
		}
	}
	return 0;
}

ClassBase *Registry::GetClassFromLabel(Label const &label)
{
	foreach (Classes::value_type &item, classes)
	{
		if (item.second->GetName().GetText() == label.GetText())
		{
			return item.second;
		}
	}
	return 0;
}

Object Registry::NewFromTypeNumber(Type::Number)
{
	return Object();
}

Object Registry::NewFromClass(ClassBase const *klass)
{
	if (klass == 0)
	{
		return Object();
	}
	Handle handle = ++next_handle;
	assert(handle != 0);
	StorageBase *base = klass->NewStorage(handle);
	instances[handle.value] = base;
	klass->SetProperties(*base);
	return *base;
}

OM_END

//EOF
