// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Registry : Value<Om::Registry>
	{
	public:
		Registry();

		Class ^GetClass(System::String ^name);
		Class ^GetClass(int type_number);

		Object ^New(Class ^klass);
		Object ^New(System::String ^name);
		Object ^New(int type_number);

		void Delete(OmNet::Object ^);

		void GarbageCollect();
	};
}

//EOF
