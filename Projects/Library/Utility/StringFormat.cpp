#include "Om/Precompiled.h"
#include "Om/Utility/StringFormat.h"

#include <cassert>
#include <stdarg.h>

std::string StringFormat(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	const int SIZ = 10*1024;
	char buf[SIZ];
#ifdef WIN32
	vsnprintf_s(buf, SIZ, SIZ, fmt, ap);
#else
	vsnprintf(buf, SIZ, fmt, ap);
#endif
	return buf;
}

void StringFormat(std::string &s, const char *fmt, ...)
{
	assert(0);
}

void StringFormat(std::vector<char> &, const char *fmt, ...)
{
	assert(0);
}

//EOF
