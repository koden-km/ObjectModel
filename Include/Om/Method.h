// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <boost/mpl/vector.hpp>
#include <boost/mpl/at.hpp>
#include <boost/mpl/size.hpp>
#include <boost/type_traits.hpp>
#include "Om/MethodBase.h"

OM_BEGIN

#include "Om/Detail/MethodPointer.h"

template <class Rty, class Klass, class Args, bool Konst>
struct Method : MethodBase, Detail::MakeMethodPointer<Rty, Klass, Args, Konst>::Type
{
	typedef typename Detail::MakeMethodPointer<Rty, Klass, Args, Konst>::Type Parent;
	Method(typename Parent::Method M) : Parent(M) 
	{ 
		signature.return_type = Type::MakeType<Rty>::Create();//Type::Traits<typename BaseType<Rty>::Type>::TypeNumber;
		signature.class_type = Type::Traits<Klass>::TypeNumber;
		signature.konst = Konst;
		Parent::AddArgs(std::back_inserter(signature.arguments));

	}
	void Invoke(Object &servant, Vector &stack) const
	{
		(*this)(servant, stack);
	}
};

#include "Om/Detail/MakeMethod.h"

OM_END

//EOF
