// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Container.h"
#include "Om/ClassBuilder.h"

OM_BEGIN

void Vector::Register(Registry &R)
{
	ClassBuilder<Vector>(R)
		.Methods
			("push_back", &Vector::push_back)
			("pop_back", &Vector::pop_back)
			("size", &Vector::size)
			("empty", &Vector::empty)
			("at", &Vector::at)
		.Properties
		;
}

void List::Register(Registry &R)
{
	ClassBuilder<List>(R)
		.Methods
			("push_back", &List::push_back)
			("pop_back", &List::pop_back)
			("size", &List::size)
			("empty", &List::empty)
		.Properties
		;
}

OM_END

//EOF
