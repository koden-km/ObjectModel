// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include "./Object.h"
#include "./Class.h"
#include "./Registry.h"
#include "./Continuation.h"
#include "./Executor.h"

using namespace System;

namespace OmNet
{
	using namespace Om;

	void Executor::PushContext(Continuation ^code)
	{
		(*value)->PushContext(code->GetValue());
	}

	void Executor::Continue()
	{
		try
		{
			(*value)->Continue();
		}
		catch (ExceptionBase &e)
		{
			throw gcnew OmException(gcnew System::String(e.ToString().c_str()));
		}
		catch (std::exception &e)
		{
			throw gcnew OmException(gcnew System::String(e.what()));
		}
		catch (...)
		{
			throw gcnew OmException("system exception");
		}
	}

	void Executor::Clear()
	{
		(*value)->Perform(OM_ROOT_NAMESPACE(Operation::Clear));
	}

	System::Collections::Generic::List<OmNet::Object ^> ^Executor::GetDataStack()
	{
		System::Collections::Generic::List<OmNet::Object ^> ^list = gcnew System::Collections::Generic::List<OmNet::Object ^>();
		foreach (OM_ROOT_NAMESPACE(Object) const &obj, (*value)->GetDataStack())
		{
			list->Add(gcnew OmNet::Object(obj));
		}
		return list;
	}
}

//EOF
