// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <sstream>
#include <string>
#include <bitset>

#include <boost/shared_ptr.hpp>
#include <boost/scoped_ptr.hpp>
#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <boost/preprocessor/stringize.hpp>

#define foreach BOOST_FOREACH
#define reverse_foreach BOOST_REVERSE_FOREACH

#include "Om/Utility/IToString.h"
#include "Om/Utility/FileLocation.h"
#include "Om/Utility/ExceptionBase.h"
#include "Om/Debug/Trace.h"

#define OM_NAMESPACE_NAME Om
#define OM_USING_NAMESPACE_OM() using namespace OM_NAMESPACE_NAME

#define OM_BEGIN namespace OM_NAMESPACE_NAME {
#define OM_END }

#define OM_ROOT_NAMESPACE(N) ::OM_NAMESPACE_NAME:: N

#include "Om/Utility/HasTraceStream.h"
#include "Om/Exceptions.h"

OM_BEGIN

struct None { };

struct Constness : IToString
{
	enum Type
	{
		Const,
		Mutable,
	};
	String ToString() const;
};

template <class T>
struct RemoveConstReference
{
	typedef typename boost::remove_reference<T>::type T0;
	typedef typename boost::remove_const<T0>::type Type;
};

//typedef std::string String;
//typedef std::stringstream StringStream;

struct Object;
struct Label;
struct StorageBase;
struct BinaryStream;
struct ReflectedBase;
struct PropertyBase;
struct MethodBase;
struct ClassBase;
template <class T>
struct Class;
struct List;
struct Vector;
struct Registry;
struct Continuation;
struct Lambda;
struct Executor;

void RegisterString(Registry&);

OM_END

//EOF
