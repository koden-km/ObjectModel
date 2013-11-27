// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

struct HasTraceStream : Optional
{
	int trace_number;

	HasTraceStream(int N, bool E = true) : Optional(E), trace_number(N) { }

	void EnableTrace(bool E = true) { Enable(E); }
	void SetEnabled(bool E) { OM_DEBUG_STREAM(trace_number).Enable(E); }
};

//EOF
