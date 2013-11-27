// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Precompiled.h"

#include "./Object.h"
#include "./Class.h"
#include "./Registry.h"
#include "./Continuation.h"
#include "./Compiler.h"

using namespace System;

namespace OmNet
{
	using namespace Om;

	Compiler::Compiler()
	{
		*value = OM_ROOT_NAMESPACE(compiler);
	}

	Continuation ^Compiler::CompileFile(System::String ^filename)
	{
		std::string str;
		MarshalString(filename, str);
		return gcnew Continuation((*value)->CompileFile(str.c_str()));
	}

	Continuation ^Compiler::CompileText(System::String ^text)
	{
		std::string str;
		MarshalString(text, str);
		return gcnew Continuation((*value)->CompileText(str.c_str()));
	}

	//OmNet::Object ^Compiler::GetCode()
	//{
	//	OM_ROOT_NAMESPACE(Object) code = (*value)->Code;
	//	OmNet::Object ^result = gcnew OmNet::Object(code.GetHandle().value, &code.GetRegistry());
	//	OM_ROOT_NAMESPACE(Object) back = result->GetObject();
	//	return result;
	//}

	void Compiler::Clear()
	{
		(*value)->Reset();
	}
}

//EOF
