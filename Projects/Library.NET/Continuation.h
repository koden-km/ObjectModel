// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Continuation : Pointer<Om::Continuation>
	{
	public:
		Continuation(Om::Pointer<Om::Continuation> const &cont)
		{
			*value = cont;
		}

		OmNet::Object ^GetCode();
		void SetScope(OmNet::Object ^);
	};
}

//EOF
