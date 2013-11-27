// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include "Om/Pathname.h"

OM_BEGIN

const boost::array<char, Pathname::Element::Last> Pathname::ElemChars = { ' ', '.', '%', '^', ' ' };

bool operator==(Pathname::Element const &A, Pathname::Element const &B) 
{ 
	bool same_type = A.type == B.type; 
	if (same_type && A.type == Pathname::Element::Ident)
		return A.label == B.label;
	return same_type;
}

bool operator<(Pathname::Element const &A, Pathname::Element const &B)
{
	return A.type < B.type || (A.type == B.type && A.label < B.label);
}

Pathname::Pathname()
{
}

Pathname::Pathname(String const &P)
{
	FromString(P);
}

bool Pathname::IsRelative() const
{
	return !absolute;
}

bool Pathname::IsAbsolute() const
{
	return absolute;
}

String Pathname::ToString() const
{
//	OM_NOT_IMPLEMENTED();
	return "";
}

void Pathname::FromString(String const &)
{
//	OM_NOT_IMPLEMENTED();
}

bool operator==(Pathname const &A, Pathname const &B)
{
	return A.absolute == B.absolute 
		&& A.elements.size() == B.elements.size() 
		&& std::equal(A.elements.begin(), A.elements.end(), B.elements.begin());
}

bool operator<(Pathname const &A, Pathname const &B)
{
	return std::lexicographical_compare(A.elements.begin(), A.elements.end(), B.elements.begin(), B.elements.end());
}

OM_END

//EOF
