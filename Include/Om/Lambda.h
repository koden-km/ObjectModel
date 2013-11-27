// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

struct BinaryStreamable
{
	virtual void Archive(BinaryStream &) = 0;
};

struct Lambda : Debug::ITraceable, BinaryStreamable
{
	typedef std::vector<Label> Labels;

	Labels capture;
	Labels arguments;
	Field<Continuation> continuation;

	void Enter(Executor &) const;

	void Archive(BinaryStream &);
	void Trace(Debug::StringStream &) const;
	static void Register(Registry &);
};

// TODO: add `BinaryStreamable` mixin
//
BinaryStream &operator<<(BinaryStream &, Lambda const &);
BinaryStream &operator>>(BinaryStream &, Lambda &);

OM_TRAITS_OPS(Lambda, Operations::Assign | Operations::Streaming);

OM_END

//EOF
