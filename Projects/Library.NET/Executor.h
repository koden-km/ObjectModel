// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Executor : Pointer<OM_ROOT_NAMESPACE(Executor)>
	{
	public:
		void Execute(Continuation ^code)
		{
			PushContext(code);
			Continue();
		}

		void PushContext(Continuation ^code);
		void Continue();

		void Clear();
		List<OmNet::Object ^> ^GetDataStack();
	};
}

//EOF
