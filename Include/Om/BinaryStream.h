// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Pointer.h"
#include "Om/Container.h"

OM_BEGIN

OM_EXCEPTION(CannotRead);
OM_EXCEPTION_1(UnknownTypeNumber, Type::Number, type_number);

/// a sequence of bytes used to store and retrieve strongly typed structured objects
struct BinaryStream : Debug::ITraceable, ReflectedBase
{
	typedef char Byte;
	typedef std::vector<Byte> Bytes;
	typedef Bytes::iterator iterator;
	typedef Bytes::const_iterator const_iterator;
	typedef Bytes::value_type value_type;

private:
	Bytes bytes;
	size_t cursor;

public:
	BinaryStream();

	size_t GetCursor() const { return cursor; }
	void ResetCursor() { cursor = 0; }

	bool CanRead(size_t num_bytes);
	int GetSize() const;

	void Write(Object const &);
	Object Read();

	bool empty() const { return bytes.empty(); }
	size_t size() const { return bytes.size(); }
	const_iterator begin() const { return bytes.begin(); }
	const_iterator end() const { return bytes.end(); }
	void clear() { bytes.clear(); cursor = 0; }

	void Write(Type::Number);
	void Write(None);
	void Write(bool);
	void Write(char);
	void Write(int);
	void Write(float);
	void Write(size_t);
	void Write(const char *);
	void Write(String const &);
	void Write(Vector const &);
	void Write(List const &);
	void Write(BinaryStream const &);

	void Read(Object &);
	void Read(Type::Number &);
	void Read(None &);
	void Read(bool &);
	void Read(char &);
	void Read(size_t&);
	void Read(int &);
	void Read(float &);
	void Read(String &);
	void Read(Vector &);
	void Read(List &);
	void Read(BinaryStream &);

	template <class T>
	void Read(Pointer<T> &X)
	{
		X = Read();
	}

	template <class T>
	void WriteSequenceContainer(SequenceContainer<T> const &C)
	{
		WritePOD(C.size());
		foreach (Object const &Q, C) 
			Write(Q);
	}
	template <class T>
	void ReadSequenceContainer(SequenceContainer<T> &C)
	{
		C.clear();
		size_t size;
		ReadPOD(size);
		for (size_t N = 0; N < size; ++N)
		{
			C.push_back(Read());
		}
	}

	friend bool operator<(BinaryStream const &, BinaryStream const &);
	friend bool operator==(BinaryStream const &, BinaryStream const &);

	void Trace(Debug::StringStream &) const;
	static void Register(Registry &);

protected:
	iterator begin() { return bytes.begin(); }
	iterator end() { return bytes.end(); }
	value_type &front() { return bytes.front(); }
	value_type &back() { return bytes.back(); }

	template <class II>
	void Write(II F, II L)
	{
		std::copy(F, L, std::back_inserter(bytes));
	}
	template <class T>
	void WriteType()
	{
		WritePOD(Type::Traits<T>::TypeNumber);
	}
	template <class T>
	void WriteTypedPOD(T const &X)
	{
		WriteType<T>();
		WritePOD(X);
	}
	template <class POD>
	void WritePOD(POD const &pod)
	{
		const size_t len = sizeof(pod);
		const size_t start = bytes.size();
		const size_t new_size = start + len;
		bytes.resize(new_size);
		memcpy(&front() + start, &pod, len);
	}
	template <class POD>
	void ReadPOD(POD &pod)
	{
		const size_t len = sizeof(pod);
		if (cursor + len > size())
		{
			OM_THROW(CannotRead);
		}
		memcpy(&pod, &front() + cursor, len);
		cursor += len;
	}
};

OM_TRAITS_OPS(BinaryStream, Operations::Streaming | Operations::Relational);

template <class T>
BinaryStream &operator<<(BinaryStream &S, T const &X)
{
	S.Write(X);
	return S;
}

template <class T>
BinaryStream &operator>>(BinaryStream &S, T &X)
{
	S.Read(X);
	return S;
}

OM_END

//EOF
