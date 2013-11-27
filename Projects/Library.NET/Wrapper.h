// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <string>

using namespace System;
using namespace System::Collections::Generic;

namespace OmNet
{
	template <class T>
	public ref class Value
	{
	public:
		typedef T Type;

		Type *value;
		Value() : value(new Type()) 
		{ 
		}
		~Value()
		{
			delete value;
		}

		Type &GetValue() { return *value; }
	};

	template <class T>
	public ref class Pointer : Value<OM_ROOT_NAMESPACE(Pointer)<T> >
	{
	};

	inline void MarshalString(System::String ^s, std::string& os) 
	{
		using namespace System::Runtime::InteropServices;
		const char* chars = (const char*)(Marshal::StringToHGlobalAnsi(s)).ToPointer();
		os = chars;
		Marshal::FreeHGlobal(System::IntPtr((void*)chars));
	}

	inline std::string ToStdString(System::String ^str)
	{
		std::string s;
		MarshalString(str, s);
		return s;
	}
}

//EOF
