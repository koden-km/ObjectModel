// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

namespace Type
{
	template <class>
	struct Traits;

	template <class T, int N, int Ops, class U = T>
	struct TraitsBase
	{
		typedef T Type;
		typedef U Storage;
		typedef U *Pointer;
		typedef U const *ConstPointer;
		typedef U &Reference;
		typedef U const &ConstReference;

		static const Number TypeNumber;
		enum { Operations = Ops };
		static const char *TypeName;

		template <int Op>
		struct HasOperation 
		{ 
			BOOST_STATIC_CONSTANT(bool, Value = (Ops & Op) != 0);
		};

		template <bool, class>
		struct BinaryOperationImpl
		{
			template <class U>
			U const &operator()(U const &, U const &)
			{
				OM_THROW_0(InvalidOperation);
			}
		};
		template <class Op>
		struct BinaryOperationImpl<true, Op> : Op { };
		template <int OpNumber, class Op>
		struct BinaryOperation : BinaryOperationImpl<HasOperation<OpNumber>::Value, Op> { };

		struct Plus : BinaryOperation<Operations::Plus, std::plus<T> > { };
		struct Minus : BinaryOperation<Operations::Minus, std::minus<T> > { };
		struct Multiply : BinaryOperation<Operations::Multiply, std::multiplies<T> > { };
		struct Divide : BinaryOperation<Operations::Divide, std::divides<T> > { };

		template <bool, class>
		struct RelationalOperationImpl
		{
			template <class U>
			bool operator()(U const &, U const &)
			{
				OM_THROW_0(InvalidOperation);
			}
		};
		template <class Op>
		struct RelationalOperationImpl<true, Op> : Op { };
		template <int OpNumber, class Op>
		struct RelationalOperation : RelationalOperationImpl<HasOperation<OpNumber>::Value, Op> { };

		struct Less : RelationalOperation<Operations::Less, std::less<T> > { };
		struct Equiv : RelationalOperation<Operations::Equiv, std::equal_to<T> > { };

		template <int Avail>
		struct StreamInsert
		{
			template <class Str, class Any>
			void operator()(Str &, Any const &)
			{
				OM_THROW_0(NoTracer);
			}
		};
		template <>
		struct StreamInsert<1>
		{
			template <class Str, class Any>
			void operator()(Str &S, Any const &X)
			{
				S << X;//Debug::Write(S, X);
			}
		};
		template <int Avail>
		struct StreamExtract
		{
			template <class Str, class Any>
			void operator()(Str &, Any const &)
			{
				OM_THROW_0(NoTracer);
			}
		};
		template <>
		struct StreamExtract<1>
		{
			template <class Str, class Any>
			void operator()(Str &S, Any &X)
			{
				S >> X;//Debug::Write(S, X);
			}
		};
		struct Tracer : StreamInsert<HasOperation<Operations::StringInsert>::Value> { };

		struct BinaryWrite : StreamInsert<HasOperation<Operations::BinaryInsert>::Value> { };
		struct BinaryRead : StreamExtract<HasOperation<Operations::BinaryExtract>::Value> { };


		template <bool>
		struct Assign
		{
			template <class A, class B>
			void operator()(A&,B const&)
			{
				OM_THROW_0(NoAssign);
			}
		};
		template <>
		struct Assign<true>
		{
			template <class A, class B>
			void operator()(A &a, B const &b)
			{
				a = b;
			}
		};
		struct Assignment : Assign<HasOperation<Operations::Assign>::Value> { };

		template <bool>
		struct BoolImpl
		{
			bool operator()(U const &X) const
			{
				OM_THROW(NoBoolConversion);
			}
		};
		template <>
		struct BoolImpl<true>
		{
			bool operator()(U const &X) const
			{
				return static_cast<bool>(X);
			}
		};
		template <bool>
		struct NotImpl
		{
			bool operator()(U const &X) const
			{
				OM_THROW(NoLogicalNegation);
			}
		};
		template <>
		struct NotImpl<true>
		{
			bool operator()(U const &X) const
			{
				return !X;
			}
		};
		struct Bool : BoolImpl<HasOperation<Operations::Bool>::Value> { };
		struct Not : NotImpl<HasOperation<Operations::Bool>::Value> { };

		template <bool>
		struct MarkImpl
		{
			template <class Any>
			void operator()(Any const &, MarkingContext &) { }
		};
		template <>
		struct MarkImpl<true>
		{
			void operator()(IMarkable const &object, MarkingContext &mark) 
			{ 
				object.SetMarked(mark);
			}
		};
		typedef typename boost::is_convertible<T *, IMarkable *> is_markable_type;
		BOOST_STATIC_CONSTANT(bool, is_markable = is_markable_type::value);
		struct Mark : MarkImpl<is_markable> { };

		template <bool>
		struct ReflectedOpsImpl
		{
			static void Create(U &, StorageBase &) { }
			template <class A>
			static void Destroy(A const &) { }
		};
		template <>
		struct ReflectedOpsImpl<true>
		{
			static void Create(ReflectedBase &R, StorageBase &S)//, OM_ROOT_NAMESPACE(Storage)<T> *S) 
			{ 
				R.Self = &S;
				R.Construct();
			}
			static void Destroy(ReflectedBase &R) 
			{ 
				R.Destroy();
			}
		};
		typedef typename boost::is_convertible<T *, ReflectedBase *> is_reflected_type;
		BOOST_STATIC_CONSTANT(bool, is_reflected = is_reflected_type::value);
		struct ReflectedOps : ReflectedOpsImpl<is_reflected> { };

	};

} // namespace Type

OM_END

//EOF
