// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Bin.h"
#include "Om/Continuation.h"
#include "Om/Lambda.h"
#include "Om/Compiler.h"
#include "Om/Runtime.h"

OM_BEGIN

void RegisterBuiltins(Registry &registry)
{
	registry.AddClass<void>();
	registry.AddClass<int>();
	registry.AddClass<float>();
	registry.AddClass<char>();
	registry.AddClass<size_t>();
	registry.AddClass<bool>();
	registry.AddClass<None>();
	registry.AddClass<Object>();
	registry.AddClass<ClassBase const *>();
	registry.AddClass<MethodBase const *>();

	RegisterString(registry);

	Vector::Register(registry);
	List::Register(registry);
	Compiler::Register(registry);
	Bin::Register(registry);
	Executor::Register(registry);
	Continuation::Register(registry);
	Lambda::Register(registry);
}

Runtime::Runtime()
{
	RegisterBuiltins(registry);

	executor = registry.New<Executor>();
	compiler = OM_ROOT_NAMESPACE(compiler);

	root = registry.New<void>();
	root["exec"] = executor;
	root["compiler"] = compiler;
	root["bin"] = root.New<Bin>();

	registry.SetRoot(root);
}

Object Runtime::Execute(String const &text)
{
	return Execute(CompileText(text));
}

Object Runtime::Execute(Pointer<Continuation> const &cont)
{
	try
	{
		executor->PushContext(cont);
		executor->Continue();
		if (!executor->GetDataStack().empty())
			return executor->GetDataStack().back();
	}
	catch (ExceptionBase &e)
	{
//		OM_TRACE_1(0, e);
		(void)e;
		throw;
	}
	return null;
}

Object Runtime::ExecuteFile(String const &filename)
{
	return Execute(CompileFile(filename));
}

Pointer<Continuation> Runtime::CompileFile(String const &filename)
{
	executor->SetSourceFile(filename);
	return compiler->CompileFile(filename);
}

Pointer<Continuation> Runtime::CompileText(String const &text)
{
	return compiler->CompileText(text);
}

OM_END

//EOF
