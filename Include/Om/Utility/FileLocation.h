#pragma once

#include "IToString.h"

struct FileLocation : /*ITraceable, */IToString
{
	const char *file;
	const char *function;
	int line;
	int column;

	FileLocation() : file(0), function(0), line(0) { }
	FileLocation(const char *F, int L, const char *G) : file(F), function(G), line(L) { }
	FileLocation(const char *F, int L, int C) : file(F), line(L), column(C) { }

	std::string ToString() const;
};

#define OM_FILE_LOCATION() \
	FileLocation(__FILE__, __LINE__, __FUNCTION__)

//EOF
