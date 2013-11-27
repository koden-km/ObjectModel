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

#include "Om/Bin.h"

using namespace System;

namespace OmNet
{
	using namespace Om;

	Runtime::Runtime()
	{
		registry = gcnew Registry();
		OM_ROOT_NAMESPACE(RegisterBuiltins(*registry->value));
		compiler = gcnew Compiler();
		executor = gcnew Executor();
		*executor->value = registry->value->New<OM_ROOT_NAMESPACE(Executor)>();
		OM_ROOT_NAMESPACE(Object) root = registry->value->New<void>();
		root["exec"] = *executor->value;
		root["compiler"] = *compiler->value;
		root["bin"] = root.New<Bin>();
		this->root = gcnew OmNet::Object(root);
		registry->value->SetRoot(root);
	}

	System::String ^OmException::ToString()
	{
		return text;
	}

}

//EOF
