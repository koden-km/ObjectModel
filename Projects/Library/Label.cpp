// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/BinaryStream.h"

OM_BEGIN

Label::Label(const char *S, bool Q) : Quoted(Q)
{
	SetText(S);
	Quoted = Q;
}

Label::Label(String const &S, bool Q) : Quoted(Q)
{
	SetText(S.c_str());
	Quoted = Q;
}

Label& Label::operator=(Label const &X)
{
	text = X.text;
	Quoted = X.Quoted;
	return *this;
}


void Label::SetText(const char *T)
{
	text = "";
	Quoted = false;
	if (!T)
		return;
	if (*T == '`')
	{
		Quoted = true;
		++T;
	}
	// TODO: check for valid identifier (ie alpha alnum*)
	text = T;
}

void Label::Trace(Debug::StringStream &S) const
{
	if (Quoted) S << '`';
	S << GetText();
}

BinaryStream& operator<<(BinaryStream& S, Label const &L)
{
	S << L.GetText();
	return S;
}

BinaryStream& operator>>(BinaryStream& S, Label &L)
{
	String text;
	S >> text;
	L = Label(text);
	return S;
}

OM_END

//EOF
