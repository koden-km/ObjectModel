// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include "./Object.h"
#include "./Class.h"

using namespace System;

namespace OmNet
{
	using namespace Om;

	System::String ^Class::GetName()
	{
		return nullptr;
	}

	System::Collections::Generic::List<Property ^> ^Class::GetProperties()
	{
		return nullptr;
	}
	System::Collections::Generic::List<Method ^> ^Class::GetMethods()
	{
		return nullptr;
	}
}

//EOF
