// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Compiler : Pointer<OM_ROOT_NAMESPACE(Compiler)>
	{
	public:
		Compiler();

		Continuation ^CompileFile(System::String ^filename);
		Continuation ^CompileText(System::String ^filename);

		void Clear();
	};
}

//EOF
