// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include "./Object.h"
#include "./Class.h"
#include "./Registry.h"
#include "./Continuation.h"
#include "./Compiler.h"
#include "./Executor.h"
#include "./Runtime.h"

void RegisterDemoTypes(Om::Registry &);

namespace OmNet
{
	using namespace Om;

	Registry::Registry()
	{
		RegisterBuiltins(*value);
		RegisterDemoTypes(*value);
	}

	Class ^Registry::GetClass(System::String ^name)
	{
		Class ^klass = gcnew Class(value->GetClassFromLabel(Label(ToStdString(name).c_str())));
		return klass;
	}

	Class ^Registry::GetClass(int type_number)
	{
		return nullptr;
	}

	System::Object ^Registry::New(Class ^klass)
	{
		return nullptr;
	}

	System::Object ^Registry::New(System::String ^name)
	{
		return nullptr;
	}

	System::Object ^Registry::New(int type_number)
	{
		return nullptr;
	}

	void Registry::Delete(OmNet::Object ^obj)
	{
		value->Delete(obj->GetHandle());
	}

	void Registry::GarbageCollect()
	{
		value->GarbageCollect();
	}
}

//EOF
