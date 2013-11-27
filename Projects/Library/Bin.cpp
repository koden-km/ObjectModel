// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/ClassBuilder.h"
#include "Om/Reflected.h"
#include "Om/Pointer.h"
#include "Om/Container.h"
#include "Om/Bin.h"

#include <iostream>

OM_BEGIN

size_t Bin::NumObjects() const
{
	return Self->GetRegistry().NumObjects();
}

void Bin::GarbageCollect() const
{
	Self->GetRegistry().GarbageCollect();
}

Object/*Pointer<BinaryStream>*/ Bin::Freeze(Object const &Q) const
{
	Pointer<BinaryStream> B = New<BinaryStream>();
	*B << Q;
	return B;
}

Object Bin::Thaw(BinaryStream &Q) const
{
	return Q.Read();
}

String Bin::ToString(Object const &Q) const
{
	return Q.ToString();
}

void Bin::EnableStream(int N, bool B) const
{
	OM_DEBUG_STREAM(N).Enable(B);
}

void Bin::Trace(Object const &Q) const
{
	Debug::StringStream S;
	Debug::Write(S, Q);
	Debug::Write(S.ToString());
}

void Bin::Print(Object const &Q) const
{
	std::cout << Q.ToString();
}

Object/*Pointer<List>*/ Bin::GetContents(Object const &Q) const
{
	std::vector<Label> names;
	Q.ForEachChild(bind(&std::vector<Label>::push_back, &names, bind(&Object::GetName, _1)));
	Pointer<Vector> ret = New<Vector>();
	foreach (Label const &label, names)
		ret->push_back(New(label));
	return ret;
}

Object/*Pointer<List> */Bin::GetMethodStrings(Object const &Q) const
{
	Pointer<Vector> strings = New<Vector>();
	if (!Q.Exists())
		return strings;
	ClassBase const *klass = Q.GetClass();
	if (!klass)
		return strings;
	foreach (ClassBase::Methods::value_type const &method, klass->GetMethods())
	{
		String text = method.second->ToString(Self->GetRegistry()) + "\n"; // hax
		strings->push_back(New(text));
	}
	return strings;
}

void Bin::Register(Registry &R)
{
	ClassBuilder<Bin>(R)
		.Methods
			("ls", &Bin::GetContents)
			("GetContents", &Bin::GetContents)
			("GetMethodStrings", &Bin::GetMethodStrings)
			("ToString", &Bin::ToString)
			("Trace", &Bin::Trace)
			("Print", &Bin::Print)
			("EnableStream", &Bin::EnableStream)
			("Freeze", &Bin::Freeze)
			("Thaw", &Bin::Thaw)
			("NumObjects", &Bin::NumObjects)
			("GarbageCollect", &Bin::GarbageCollect)
		.Properties
		;
}

void RegisterString(Registry& R)
{
	ClassBuilder<String>(R)
		.Methods
			("size", &String::size)
			("empty", &String::empty)
		.Properties
		;
}

OM_END

//EOF
