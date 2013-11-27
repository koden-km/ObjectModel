// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

#define OM_TRAITS_NUMBER_NAME_OPS_STORE(Ty, Num, Name, Ops, Store) \
	namespace Type \
	{ \
		template <> \
		struct Traits<Ty> : TraitsBase<Ty, Num, Ops, Store> \
		{ \
		}; \
		template <> \
		const char *TraitsBase<Ty, Num, Ops, Store> ::TypeName = Name; \
		template <> \
		const OM_ROOT_NAMESPACE(Type::Number) TraitsBase<Ty, Num, Ops, Store> ::TypeNumber = Num; \
	}

#define OM_TRAITS_NUMBER_NAME_OPS(Ty, Num, Name, Ops) \
	OM_TRAITS_NUMBER_NAME_OPS_STORE(Ty, Num, Name, Ops, Ty)

#define OM_TYPE_NUMBER(Ty) \
	OM_ROOT_NAMESPACE(Type::Number:: Ty)

#define OM_TRAITS(Ty) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, OM_TYPE_NUMBER(Ty), BOOST_PP_STRINGIZE(Ty), Operations::None)

#define OM_TRAITS_NUMBER(Ty, Num) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, Num, BOOST_PP_STRINGIZE(Ty), Operations::None)

#define OM_TRAITS_NUMBER_NAME(Ty, Num, Name) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, Num, Name, Operations::None)

#define OM_TRAITS_OPS(Ty, Ops) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, OM_TYPE_NUMBER(Ty), BOOST_PP_STRINGIZE(Ty), Ops)

#define OM_TRAITS_NAME(Ty, Name) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, OM_TYPE_NUMBER(Ty), Name, Operation::None)

#define OM_TRAITS_NUMBER_OPS(Ty, Num, Ops) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, Num, BOOST_PP_STRINGIZE(Ty), Ops)

#define OM_TRAITS_NAME_OPS(Ty, Name, Ops) \
	OM_TRAITS_NUMBER_NAME_OPS(Ty, OM_TYPE_NUMBER(Ty), Name, Ops)

OM_END

#define USER_TYPE_NUMBER(Ty, Num) \
	OM_BEGIN\
	OM_TRAITS_NUMBER(Ty, Num); \
	OM_END

//EOF
