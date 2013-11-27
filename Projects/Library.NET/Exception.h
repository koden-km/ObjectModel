// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class OmException : public System::Exception
	{
		System::String ^text;

	public:
		OmException(System::String ^S) : text(S) { }

		virtual System::String ^ToString() override;
	};
}

//EOF
