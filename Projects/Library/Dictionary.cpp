// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Dictionary.h"

OM_BEGIN

StringStream& operator<<(StringStream& S, Type::Number N)
{
	S << "TypeNumber " << N.value;
	return S;
}

void Dictionary::Add(Label const &label, Object const &object)
{
	objects[label] = object;
}

Object Dictionary::Find(Label const &label) const
{
	Objects::const_iterator iter = objects.find(label);
	return iter == objects.end() ? Object() : iter->second;
}

void Dictionary::Erase(Label const &label)
{
	Objects::const_iterator iter = objects.find(label);
	if (iter == objects.end())
		return;
	objects.erase(iter);
}

void TraceDictionary(Object const &Q)
{
	//OM_TRACE_1(0, Q);
	if (!Q)
	{
		OM_TRACE(0) << "null";
		return;
	}
	OM_TRACE_1(0, Q.GetStorage().GetDictionary().size());
	foreach (Dictionary::value_type const &val, Q.GetStorage().GetDictionary())
	{
		Label const &label = val.first;
		//Object const &child = val.second;
		OM_TRACE_1(0, label);//, child);
	}
}

OM_END

//EOF
