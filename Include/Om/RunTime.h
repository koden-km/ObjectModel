// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/ObjectModel.h"
#include "Om/Compiler.h"

OM_BEGIN

struct Compiler;
struct Executor;

/// everything required to compile, execute and store the reflected object model 
struct Runtime
{
	Pointer<Compiler> compiler;
	Pointer<Executor> executor;
	Object root;
	Registry registry;

	Runtime();

	Object Execute(String const &);
	Object ExecuteFile(String const &);
	Object Execute(Pointer<Continuation> const &);
	Object Continue(Pointer<Continuation> const &C) { return Execute(C); }

	Pointer<Continuation> Compile(String const &text) { return CompileText(text); }
	Pointer<Continuation> CompileFile(String const &);
	Pointer<Continuation> CompileText(String const &);

	template <class K>
	void Register()
	{
		K::Register(registry);
	}

	template <class T>
	Object New()
	{
		return registry.New<T>();
	}
	template <class T>
	Object New(T const &X)
	{
		return registry.New(X);
	}
};

void RegisterBuiltins(Registry &registry);

OM_END

//EOF
