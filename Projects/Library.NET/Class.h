// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Method
	{
		//TODO
	};

	public ref class Property
	{
		//TODO
	};

	public ref class Class
	{
		Om::ClassBase const *klass;

	public:
		Class(Om::ClassBase const *K) : klass(K) { }

		System::String ^GetName();
		List<Property ^> ^GetProperties();
		List<Method ^> ^GetMethods();
	};
}

//EOF
