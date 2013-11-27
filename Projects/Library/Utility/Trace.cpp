#include "Om/Precompiled.h"
#include <string>
#include <vector>
#include <stdarg.h>

#include "Om/Utility/Foreach.h"
#include "Om/Utility/NonCopyable.h"

#include "Om/Utility/BaseBase.h"
#include "Om/Utility/Optional.h"

#include "Om/Debug/Trace.h"

#ifdef WIN32
#define WIN32_LEAN_AND_MEAN
#include "Om/Utility/WindowsHeaders.h"
#endif

namespace Debug
{
	StreamBase::StreamBase(bool A)
		: Optional(A), indent_level(0)
	{
	}

	StreamBase::~StreamBase()
	{
	}

	void StreamBase::NewLine()
	{
		Write("\n");
		foreach (int n, range(0, indent_level))
		{
			Write("\t");
		}
	}

	void StreamBase::Indent()
	{
		++indent_level;
	}

	void StreamBase::Outdent()
	{
		assert(indent_level > 0);
		--indent_level;
	}

	endl_struct endl;
	indent_struct indent;
	outdent_struct outdent;
	param_sep_struct param_sep;

	boost::array<StringStream, 20> debug_streams;

	Printer::Printer(StringStream &S, FileLocation const &FL) 
		: input(S)
	{ 
#ifdef OM_TRACE_ENABLE
		total << FL << indent << endl << input;
#endif
	}

	Printer::~Printer()
	{
#ifdef OM_TRACE_ENABLE
		total << outdent << endl;
		if (input.Enabled())
			Write(total.ToString());
		input.Clear();
#endif
	}
	Scoped::Scoped(::Debug::StringStream &S, const char *N) 
		: stream(S)
		, name(N)
	{
#ifdef OM_TRACE_ENABLE
		stream << name << endl << "{" << indent << endl;
#endif
	}

	Scoped::~Scoped()
	{
#ifdef OM_TRACE_ENABLE
		stream << outdent << endl << "}";// << endl;
#endif
	}

	StringStream &GetStream(size_t N)//, FileLocation const &FL)
	{
		StringStream &stream = debug_streams[N];
#ifdef OM_TRACE_ENABLE
		stream << std::boolalpha;
#endif
		return debug_streams[N];
	}

	void EnableStream(size_t N, bool active)
	{
#ifdef OM_TRACE_ENABLE
		GetStream(N).Enable(active);
#endif
	}

	void Write(ITraceable const &T)
	{
#ifdef OM_TRACE_ENABLE
		StringStream S;
		T.Trace(S);
		Write(S);
#endif
	}

	void DebugWrite(const char *text)
	{
#ifdef OM_TRACE_ENABLE
#	ifdef WIN32
		OutputDebugStringA(text);
#	else
		fprintf(stderr, "%s", text);
#	endif
#endif
	}

	void Write(IToString const &T)
	{
		DebugWrite(T.ToString().c_str());
	}

	void Write(std::string const &T)
	{
		DebugWrite(T.c_str());
	}

	void Write(const char *fmt, ...)
	{
#ifdef OM_TRACE_ENABLE
		va_list ap;
		va_start(ap, fmt);
		const int SIZ = 10*1024;
		char buf[SIZ];
#ifdef WIN32
		vsnprintf_s(buf, SIZ, SIZ, fmt, ap);
#else
		vsprintf(buf, fmt, ap);
#endif
		DebugWrite(buf);
#endif

	}
	void WriteLine(const char *fmt, ...)
	{
#ifdef OM_TRACE_ENABLE
		va_list ap;
		va_start(ap, fmt);
		const int SIZ = 10*1024;
		char buf[SIZ];
#ifdef WIN32
		vsnprintf_s(buf, SIZ, SIZ, fmt, ap);
#else
		vsprintf(buf, fmt, ap);
#endif
		DebugWrite(buf);
		DebugWrite("\n");
#endif

	}
	void Write(StringStream &, std::wstring const &)
	{
		// TODO
	}

} // namespace Debug

//EOF
