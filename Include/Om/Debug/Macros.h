#pragma once

#define OM_TRACE_PRINT(S) \
	::Debug::Write(S)

#define OM_DEBUG_STREAM(N) \
	::Debug::GetStream(N)

#define OM_DEBUG_STREAM_FL(N) \
	(OM_DEBUG_STREAM(N) << OM_FILE_LOCATION())

#define OM_SCOPED_TRACE(S, name) \
	::Debug::Scoped OM_UNIQUE_FILE_IDENT(scoped_trace)(S, name)

#define OM_TRACE_PRINTER(S) \
	::Debug::Printer/*<::Debug::Trace::StringStream>*/(S, OM_FILE_LOCATION())

#define OM_TRACE_OBJECT(Q) \
	::Debug::Write(OM_FILE_LOCATION()); \
	::Debug::Write(Q)


#define OM_TRACE_PARAM(P) \
	BOOST_PP_STRINGIZE(P) << "=" << P

#define OM_TRACE_STREAM_0(S) \
	S

#define OM_TRACE_STREAM_1(S, A0) \
	S << OM_TRACE_PARAM(A0)

#define OM_TRACE_STREAM_2(S, A0, A1) \
	OM_TRACE_STREAM_1(S,A0) << param_sep << OM_TRACE_PARAM(A1)

#define OM_TRACE_STREAM_3(S, A0, A1, A2) \
	OM_TRACE_STREAM_2(S,A0,A1) << param_sep << OM_TRACE_PARAM(A2)

#define OM_TRACE_STREAM_4(S, A0, A1, A2, A3) \
	OM_TRACE_STREAM_3(S,A0,A1,A2) << param_sep << OM_TRACE_PARAM(A3)

#define OM_TRACE_STREAM_5(S, A0, A1, A2, A3, A4) \
	OM_TRACE_STREAM_4(S,A0,A1,A2,A3) << param_sep << OM_TRACE_PARAM(A4)

#define OM_TRACE_STREAM_6(S, A0, A1, A2, A3, A4, A5) \
	OM_TRACE_STREAM_5(S,A0,A1,A2,A3,A4) << param_sep << OM_TRACE_PARAM(A5)

#define OM_TRACE(N) \
	OM_TRACE_PRINTER(OM_DEBUG_STREAM(N))

#define OM_TRACE_0(N) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_0(OM_DEBUG_STREAM(N)))

#define OM_TRACE_1(N, A0) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_1(OM_DEBUG_STREAM(N), A0))

#define OM_TRACE_2(N, A0, A1) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_2(OM_DEBUG_STREAM(N), A0, A1))

#define OM_TRACE_3(N, A0, A1, A2) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_3(OM_DEBUG_STREAM(N), A0, A1, A2))

#define OM_TRACE_4(N, A0, A1, A2, A3) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_4(OM_DEBUG_STREAM(N), A0, A1, A2, A3))

#define OM_TRACE_5(N, A0, A1, A2, A3, A4) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_5(OM_DEBUG_STREAM(N), A0, A1, A2, A3, A4))

#define OM_TRACE_6(N, A0, A1, A2, A3, A4, A5) \
	OM_TRACE_PRINTER(OM_TRACE_STREAM_6(OM_DEBUG_STREAM(N), A0, A1, A2, A3, A4, A5))

#define OM_UNIQUE_FILE_IDENT(name) \
	BOOST_PP_CAT(name, __LINE__)

//---------------------------------------------------------------------------
// OM_TRACE_OBJECT_N is used to trace out a set of arguments 
// grouped by delimiters
//---------------------------------------------------------------------------

#define OM_TRACE_OBJECT__(name, S, TRACE) \
	OM_SCOPED_TRACE(S, name);\
	TRACE

#define OM_TRACE_OBJECT_0(name, S) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_0(S))

#define OM_TRACE_OBJECT_1(name, S, A0) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_1(S,A0))

#define OM_TRACE_OBJECT_2(name, S, A0, A1) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_2(S,A0,A1))

#define OM_TRACE_OBJECT_3(name, S, A0, A1, A2) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_3(S,A0,A1,A2))

#define OM_TRACE_OBJECT_4(name, S, A0, A1, A2, A3) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_4(S,A0,A1,A2,A3))

#define OM_TRACE_OBJECT_5(name, S, A0, A1, A2, A3, A4) \
	OM_TRACE_OBJECT__(name, S, OM_TRACE_STREAM_5(S,A0,A1,A2,A3,A4))

//EOF
