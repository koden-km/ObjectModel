// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include "./Object.h"
#include "./Class.h"
#include "./Registry.h"

using namespace System;

namespace OmNet
{
	using namespace Om;

	Object::Object()
		: handle(0), registry(0)
	{
	}

	Object::Object(OM_ROOT_NAMESPACE(Object) const &Q)
		: handle(Q.GetHandle().value), registry(&Q.GetRegistry())
	{
	}

	Object::Object(int H, OM_ROOT_NAMESPACE(Registry) *R)
		: handle(H), registry(R)
	{
	}

	int Object::GetTypeNumber()
	{
		return GetObject().GetTypeNumber().value;
	}

	OM_ROOT_NAMESPACE(Object) Object::GetObject()
	{
		return registry->GetObject(handle);
	}

	System::String ^Object::ToString()
	{
		return gcnew System::String(GetObject().ToString().c_str());
	}

	System::Collections::Generic::List<Object ^> ^Object::GetChildren()
	{
		return nullptr;
	}

}

//EOF
