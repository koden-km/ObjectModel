// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/BinaryStream.h"
#include "Om/ClassBuilder.h"

#include <boost/numeric/conversion/cast.hpp>

OM_BEGIN

BinaryStream::BinaryStream()
	: cursor(0)
{
}

bool BinaryStream::CanRead(size_t num_bytes)
{
	return cursor + num_bytes < bytes.size();
}

void BinaryStream::Write(Type::Number tn)
{
	WritePOD(tn);
}

void BinaryStream::Write(None)
{
	Write(Type::Traits<None>::TypeNumber);
}

void BinaryStream::Write(bool X)
{
	WritePOD(X);
}

void BinaryStream::Write(char X)
{
	WritePOD(X);
}

void BinaryStream::Write(int X)
{
	WritePOD(X);
}

void BinaryStream::Write(size_t X)
{
	WritePOD(X);
}

void BinaryStream::Write(float X)
{
	WritePOD(X);
}

void BinaryStream::Write(String const &S)
{
	WritePOD(S.size());
	Write(S.begin(), S.end());
}

void BinaryStream::Write(const char *S)
{
	size_t len = strlen(S);
	Write(len);
	Write(S, S + len);
}

void BinaryStream::Write(Vector const &V)
{
	WriteSequenceContainer(V);
}

void BinaryStream::Write(List const &V)
{
	WriteSequenceContainer(V);
}

void BinaryStream::Read(Type::Number &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(None &)
{
}

void BinaryStream::Read(bool &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(char &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(int &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(size_t &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(float &X)
{
	ReadPOD(X);
}

void BinaryStream::Read(String &S)
{
	size_t len;
	ReadPOD(len);
	S.resize(len);
	if (len == 0)
		return;
	memcpy(&*S.begin(), &front() + cursor, len);
	cursor += len;
}

void BinaryStream::Read(Vector &C)
{
	ReadSequenceContainer(C);
}

void BinaryStream::Read(List &C)
{
	ReadSequenceContainer(C);
}

void BinaryStream::Write(BinaryStream const &B)
{
	WriteType<BinaryStream>();
	WritePOD(B.size());
	Write(B.begin(), B.end());
}

void BinaryStream::Read(BinaryStream &B)
{
	B.clear();
	size_t len;
	ReadPOD(len);
	B.bytes.resize(len);
	memcpy(&B.front(), &front() + cursor, len);
	cursor += len;
}

void BinaryStream::Write(Object const &Q)
{
	if (!Q)
	{
		WriteType<None>();
		return;
	}
	ClassBase const *klass = Q.GetClass();
	StorageBase const &base = Q.GetStorage();
	Dictionary const &dict = base.GetDictionary();

	Write(klass->GetTypeNumber());

	// write the value of the object, if possible
	if (klass->HasOperation(Type::Operations::BinaryInsert))
		klass->Write(*this, Q);
	
	// write the number of sub-objects
	Write(dict.size() + klass->GetProperties().size());

	// write the child objects
	foreach (Dictionary::Objects::value_type const &val, dict)
	{
		Write(val.first.ToString().c_str());
		Write(val.second);
	}

	// write the properties
	foreach (ClassBase::Properties::value_type const &iter, klass->GetProperties())
	{
		Write(iter.first.ToString().c_str());
		Write(iter.second->Get(base));
	}
}

void BinaryStream::Read(Object &Q)
{
	Type::Number type_number;
	ReadPOD(type_number);
	ClassBase const *klass = Self->GetRegistry().GetClass(type_number);
	if (klass == 0)
	{
		OM_THROW_1(UnknownTypeNumber, type_number);
		return;
	}

	Q = klass->Create();

	// read the object value, if possible
	if (klass->HasOperation(Type::Operations::BinaryExtract))
		klass->Read(*this, Q);

	// read the sub-objects; children and properties are handled the same way
	size_t num_sub_objects = 0;
	Read(num_sub_objects);
	for (size_t N = 0; N < num_sub_objects; ++N)
	{
		String name;
		Read(name);
		Q.Set(name, Read());
	}
}

Object BinaryStream::Read()
{
	Object object;
	Read(object);
	return object;
}

int BinaryStream::GetSize() const
{
	return boost::numeric_cast<int>(bytes.size());
}

bool operator==(BinaryStream const &A, BinaryStream const &B)
{
	return A.GetSize() == B.GetSize();
}

bool operator<(BinaryStream const &A, BinaryStream const &B)
{
	return A.GetSize() < B.GetSize();
}

void BinaryStream::Trace(Debug::StringStream &S) const
{
	S << "BinaryStream: " << GetSize() << "bytes";
}

void BinaryStream::Register(Registry &R)
{
	// disambiguate the methods to expose
	void (BinaryStream::*Write)(Object const &) = &BinaryStream::Write;
	Object (BinaryStream::*Read)() = &BinaryStream::Read;

	ClassBuilder<BinaryStream>(R)
		.Methods
			("Write", Write)
			("Read", Read)
			("GetSize", &BinaryStream::GetSize)
		.Properties
		;
}

OM_END

//EOF
