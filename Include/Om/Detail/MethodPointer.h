// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Pointer.h"

namespace Detail
{

	template <bool IsKonst, bool IsVoid, int Arity, class Return, class Klass, class Args>
	struct MethodPointer;


	//----------------------------------------------------------- arity = 0
	struct MethodPointerBase0
	{
		template <class OI>
		void AddArgs(OI)
		{
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, false, 0, Return, Klass, Args> : MethodPointerBase0
	{
		typedef Return (Klass::*Method)();
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			stack.push_back(object.New((Deref<Klass>(object).*method)()));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, false, 0, Return, Klass, Args> : MethodPointerBase0
	{
		typedef Return (Klass::*Method)() const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			stack.push_back(object.New((Deref<Klass>(object).*method)()));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, true, 0, Return, Klass, Args> : MethodPointerBase0
	{
		typedef Return (Klass::*Method)();
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			(Deref<Klass>(object).*method)();
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, true, 0, Return, Klass, Args> : MethodPointerBase0
	{
		typedef Return (Klass::*Method)() const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			(Deref<Klass>(object).*method)();
		}
	};

	//----------------------------------------------------------- arity = 1
	template <class Args>
	struct MethodPointerBase1 : MethodPointerBase0
	{
		typedef typename boost::mpl::at_c<Args, 0>::type A0;
		typedef typename BaseType<A0>::Type B0;
		typedef Pointer<B0> P0;
		template <class OI>
		void AddArgs(OI P)
		{
			MethodPointerBase0::AddArgs(P);
			*P++ = Type::MakeType<A0>::Create();
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, false, 1, Return, Klass, Args> : MethodPointerBase1<Args>
	{
		typedef Return (Klass::*Method)(A0);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, false, 1, Return, Klass, Args> : MethodPointerBase1<Args>
	{
		typedef Return (Klass::*Method)(A0) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, true, 1, Return, Klass, Args> : MethodPointerBase1<Args>
	{
		typedef Return (Klass::*Method)(A0);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0);
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, true, 1, Return, Klass, Args> : MethodPointerBase1<Args>
	{
		typedef Return (Klass::*Method)(A0) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0);
		}
	};

	//----------------------------------------------------------- arity = 2
	template <class Args>
	struct MethodPointerBase2 : MethodPointerBase1<Args>
	{
		typedef typename boost::mpl::at_c<Args, 1>::type A1;
		typedef typename BaseType<A1>::Type B1;
		typedef Pointer<B1> P1;
		template <class OI>
		void AddArgs(OI P)
		{
			MethodPointerBase1<Args>::AddArgs(P);
			*P++ = Type::MakeType<A1>::Create();
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, false, 2, Return, Klass, Args> : MethodPointerBase2<Args>
	{
		typedef Return (Klass::*Method)(A0,A1);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0, *a1)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, false, 2, Return, Klass, Args> : MethodPointerBase2<Args>
	{
		typedef Return (Klass::*Method)(A0,A1) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0, *a1)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, true, 2, Return, Klass, Args> : MethodPointerBase2<Args>
	{
		typedef Return (Klass::*Method)(A0,A1);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0, *a1);
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, true, 2, Return, Klass, Args> : MethodPointerBase2<Args>
	{
		typedef Return (Klass::*Method)(A0, A1) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0, *a1);
		}
	};
	//----------------------------------------------------------- arity = 3
	template <class Args>
	struct MethodPointerBase3 : MethodPointerBase2<Args>
	{
		typedef typename boost::mpl::at_c<Args, 2>::type A2;
		typedef typename BaseType<A2>::Type B2;
		typedef Pointer<B2> P2;
		template <class OI>
		void AddArgs(OI P)
		{
			MethodPointerBase2<Args>::AddArgs(P);
			*P++ = Type::MakeType<A2>::Create();
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, false, 3, Return, Klass, Args> : MethodPointerBase3<Args>
	{
		typedef Return (Klass::*Method)(A0,A1,A2);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P2 a2 = stack.pop();
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0, *a1, *a2)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, false, 3, Return, Klass, Args> : MethodPointerBase3<Args>
	{
		typedef Return (Klass::*Method)(A0,A1,A2) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P2 a2 = stack.pop();
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			stack.push_back(object.New((Deref<Klass>(object).*method)(*a0, *a1, *a2)));
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<false, true, 3, Return, Klass, Args> : MethodPointerBase3<Args>
	{
		typedef Return (Klass::*Method)(A0,A1,A2);
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object &object, Vector &stack) const
		{
			P2 a2 = stack.pop();
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0, *a1, *a2);
		}
	};
	template <class Return, class Klass, class Args>
	struct MethodPointer<true, true, 3, Return, Klass, Args> : MethodPointerBase3<Args>
	{
		typedef Return (Klass::*Method)(A0, A1,A2) const;
		Method method;
		MethodPointer(Method M) : method(M) { }
		void operator()(Object const &object, Vector &stack) const
		{
			P2 a2 = stack.pop();
			P1 a1 = stack.pop();
			P0 a0 = stack.pop();
			(Deref<Klass>(object).*method)(*a0, *a1, *a2);
		}
	};

	template <class Rty, class Klass, class Args, bool Konst>
	struct MakeMethodPointer
	{
		typedef MethodPointer<Konst, boost::is_same<void, Rty>::value, boost::mpl::size<Args>::value, Rty, Klass, Args> Type;
	};
}

//EOF
