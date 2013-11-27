// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Reflected.h"
#include "Om/Compiler.h"
#include "Om/Bin.h"
#include "Om/Runtime.h"

#include <iostream>
#include <stdarg.h>

OM_USING_NAMESPACE_OM();

void TraceStack(Vector const &list);
void Execute(Runtime &runtime, Pointer<Continuation>);
void RegisterDemoTypes(Registry &);

int main(int argc, char **argv)
{
	Runtime runtime;
	RegisterDemoTypes(runtime.registry);

	bool debug = true;
	bool trace_exec = false;
	bool run = true;

	if (argc == 2)
	{
		Execute(runtime, runtime.compiler->CompileFile(argv[1]));
	}

	char buffer[2000];
	std::cout << runtime.registry.NumObjects() << "> ";
	while (std::cin.getline(buffer, 2000))
	{
		*runtime.compiler->PrintTree = debug;
		try
		{
			runtime.Execute(buffer);
		}
		catch (CompileError &e)
		{
			StringStream msg;
			msg << "CompileError: " << e.MessageText() << "\n";
			Debug::Write(msg.str().c_str());
		}
		catch (ExceptionBase &e)
		{
			OM_TRACE(2) << e << endl;
		}
		catch (std::exception &e)
		{
			OM_TRACE(2) << e.what() << endl;
		}

		TraceStack(runtime.executor->GetDataStack());

		runtime.registry.GarbageCollect();

		std::cout << runtime.registry.NumObjects() << "> ";
	}

    return 0;
}

void Execute(Runtime &runtime, Pointer<Continuation> continuation)
{
	try
	{
		runtime.executor->Continue(continuation);
	}
	catch (ExceptionBase &e)
	{
		//OM_TRACE(2) << e << endl;
		Debug::Write(e.ToString() + "\n");
	}
	catch (std::exception &e)
	{
		OM_TRACE(2) << e.what() << endl;
	}
}

void TraceStack(Vector const &list)
{
	Debug::StringStream S;
	int N = 0;
	foreach (Object const &object, list.container)
	{
		try
		{
			S << "[" << N++ << "] " << object << endl;
		}
		catch (ExceptionBase &e)
		{
			S << "[" << N++ << "] " << e << endl;
		}
	}
	std::cout << S.ToString();;
}

//EOF
