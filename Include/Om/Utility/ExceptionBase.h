#pragma once

#include <stdexcept>
#include <string>
#include <sstream>
#include <boost/preprocessor/stringize.hpp>

#include "BaseBase.h"
#include "Optional.h"
#include "Debug/Trace.h"
#include "StringStream.h"
#include "FileLocation.h"

//typedef std::string String;

struct ExceptionBase : std::exception, IToString
{
	FileLocation file_location;
	const char *name;

	ExceptionBase(FileLocation const &fl, const char *N) : file_location(fl), name(N) { }
	virtual ~ExceptionBase( ) throw() { }

	virtual std::string MessageText() const { return ""; }
	std::string ToString() const;
};

#define OM_THROW_EX_0(FL, E) \
	throw E(FL);

#define OM_THROW_EX_1(FL, E, A0) \
	throw E(FL, A0);

#define OM_THROW_EX_2(FL, E, A0, A1) \
	throw E(FL, A0, A1);

#define OM_THROW_EX_3(FL, E, A0, A1, A2) \
	throw E(FL, A0, A1, A2);

#define OM_THROW(E) \
	OM_THROW_0(E)

#define OM_THROW_0(E) \
	OM_THROW_EX_0(OM_FILE_LOCATION(), E)

#define OM_THROW_1(E, A0) \
	OM_THROW_EX_1(OM_FILE_LOCATION(), E, A0)

#define OM_THROW_2(E, A0, A1) \
	OM_THROW_EX_2(OM_FILE_LOCATION(), E, A0, A1)

#define OM_THROW_3(E, A0, A1, A2) \
	OM_THROW_EX_3(OM_FILE_LOCATION(), E, A0, A1, A2)

#define OM_EXCEPTION(Name) \
	struct Name : ExceptionBase \
	{ \
		Name(FileLocation const &FL) : ExceptionBase(FL, BOOST_PP_STRINGIZE(Name)) { } \
	}

#define OM_EXCEPTION_1(Name, Type, Field) \
	struct Name : ExceptionBase \
	{ \
		Type Field; \
		Name(FileLocation const &FL, Type const &X) : ExceptionBase(FL, BOOST_PP_STRINGIZE(Name)), Field(X) { } \
		String MessageText() const \
		{  \
			::Debug::StringStream S; \
			OM_TRACE_STREAM_1(S, Field); \
			return S.ToString(); \
		} \
	}

#define OM_EXCEPTION_2(Name, Type, Field, Type2, Field2) \
	struct Name : ExceptionBase \
	{ \
		Type Field; \
		Type2 Field2; \
		Name(FileLocation const &FL, Type const &X, Type2 const &Y) \
			: ExceptionBase(FL, BOOST_PP_STRINGIZE(Name)), Field(X), Field2(Y) { } \
		String MessageText() const \
		{  \
			::Debug::StringStream S; \
			OM_TRACE_STREAM_2(S, Field, Field2); \
			return S.ToString(); \
		} \
	}

//EOF
