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

	OmNet::Object ^Continuation::GetCode()
	{
		return gcnew OmNet::Object((*value)->Code);
	}

	void Continuation::SetScope(OmNet::Object ^scope)
	{
		(*value)->Scope = scope->GetObject();
	}

}

//EOF

