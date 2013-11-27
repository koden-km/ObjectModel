// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	public ref class Object
	{
	private:
		int handle;
		OM_ROOT_NAMESPACE(Registry) *registry;

	public:
		Object();
		Object(OM_ROOT_NAMESPACE(Object) const &);
		Object(int handle, OM_ROOT_NAMESPACE(Registry) *registry);

		int GetTypeNumber();
		int GetHandle() { return handle; }
		System::String ^ToString() new;
		List<OmNet::Object ^> ^GetChildren();

	//protected:
		OM_ROOT_NAMESPACE(Object) GetObject();
	};
}

//EOF
