#include "Om/Precompiled.h"
#include "Om/Utility/ExceptionBase.h"
#include "Om/Utility/FileLocation.h"

std::string FileLocation::ToString() const
{
	if (file == 0)
	{
		return "(Null Filelocation)";
	}
	StringStream S;
	if (function == 0)
	{
		S << file << "(" << line << "): ";
	}
	else
	{
		S << file << "(" << line << "): " << function << " ";
	}
	return S.str();
}

std::string ExceptionBase::ToString() const
{
	StringStream S;
	S << file_location.ToString() << ": Exception: " << name << ": " << MessageText();
	return S.str();
}

//EOF
