// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace OmNet
{
	/// everything needed to dynamically compile, link and run objects reflected from C++ into .NET
	public ref class Runtime
	{
	public:
		Compiler ^compiler;
		Executor ^executor;
		OmNet::Object ^root;
		Registry ^registry;

		Runtime();
	};
}

//EOF
