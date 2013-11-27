// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

#pragma warning(push)
//warning C4800: 'const int' : forcing value to bool 'true' or 'false' (performance warning)
#pragma warning(disable:4800)

OM_TRAITS_NUMBER_NAME_OPS_STORE(void, Number::Void, "Void", Operations::None, None);

//OM_TRAITS_NUMBER_NAME(OM_ROOT_NAMESPACE(Type::Number), Number::TypeNumber, "TypeNumber");

OM_TRAITS(None);

OM_TRAITS_NUMBER_NAME_OPS(bool, Number::Bool, "bool", Operations::Equiv | Operations::Assign | Operations::Streaming | Operations::Boolean);

OM_TRAITS_NUMBER_OPS(char, Number::Char, Operations::All);
OM_TRAITS_NUMBER_OPS(int, Number::Int, Operations::All);
OM_TRAITS_NUMBER_OPS(size_t, Number::Size, Operations::All);

OM_TRAITS_NUMBER_OPS(float, Number::Float, Operations::All);

OM_TRAITS_OPS(String, Operations::Streaming | Operations::Plus | Operations::Relational | Operations::Assign);

#pragma warning(pop)

OM_END

//EOF
