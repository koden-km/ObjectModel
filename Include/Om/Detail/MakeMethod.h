// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

namespace Detail
{
	// ---------------------------------- arity = 0

	template <class Klass>
	MethodBase *MakeMethod(void (Klass::*M)())
	{
		return new Method<void, Klass, boost::mpl::vector<>, false>(M);
	}

	template <class R, class Klass>
	MethodBase *MakeMethod(R (Klass::*M)())
	{
		return new Method<R, Klass, boost::mpl::vector<>, false>(M);
	}

	template <class Klass>
	MethodBase *MakeMethod(void (Klass::*M)() const)
	{
		return new Method<void, Klass, boost::mpl::vector<>, true>(M);
	}

	template <class R, class Klass>
	MethodBase *MakeMethod(R (Klass::*M)() const)
	{
		return new Method<R, Klass, boost::mpl::vector<>, true>(M);
	}

	// ---------------------------------- arity = 1

	template <class Klass, class R, class A0>
	MethodBase *MakeMethod(R (Klass::*M)(A0))
	{
		return new Method<R, Klass, boost::mpl::vector<A0>, false>(M);
	}

	template <class Klass, class R, class A0>
	MethodBase *MakeMethod(R (Klass::*M)(A0) const)
	{
		return new Method<R, Klass, boost::mpl::vector<A0>, true>(M);
	}

	// ---------------------------------- arity = 2

	template <class Klass, class R, class A0, class A1>
	MethodBase *MakeMethod(R (Klass::*M)(A0,A1))
	{
		return new Method<R, Klass, boost::mpl::vector<A0,A1>, false>(M);
	}

	template <class Klass, class R, class A0, class A1>
	MethodBase *MakeMethod(R (Klass::*M)(A0,A1) const)
	{
		return new Method<R, Klass, boost::mpl::vector<A0,A1>, true>(M);
	}

	// ---------------------------------- arity = 3

	template <class Klass, class R, class A0, class A1, class A2>
	MethodBase *MakeMethod(R (Klass::*M)(A0,A1,A2))
	{
		return new Method<R, Klass, boost::mpl::vector<A0,A1,A2>, false>(M);
	}

	template <class Klass, class R, class A0, class A1, class A2>
	MethodBase *MakeMethod(R (Klass::*M)(A0,A1,A2) const)
	{
		return new Method<R, Klass, boost::mpl::vector<A0,A1,A2>, true>(M);
	}
}

//EOF
