// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/PropertyBase.h"
#include "Om/MethodBase.h"
#include "Om/ClassBase.h"

OM_BEGIN

void ClassBase::AddProperty(Label const &label, PropertyBase *prop)
{
	prop->SetLabel(label);
	properties[label] = prop;
}

void ClassBase::AddMethod(Label const &label, MethodBase *method)
{
	method->SetLabel(label);
	methods[label] = method;
}

MethodBase const *ClassBase::GetMethod(struct Label const &label) const
{
	Methods::const_iterator method = methods.find(label);
	return method == methods.end() ? 0 : method->second;
}

PropertyBase const *ClassBase::GetProperty(Label const &label) const
{
	Properties::const_iterator prop = properties.find(label);
	return prop == properties.end() ? 0 : prop->second;
}

void ClassBase::Trace(Debug::StringStream &S) const
{
	S << "Class " << name;
}

OM_END

//EOF
