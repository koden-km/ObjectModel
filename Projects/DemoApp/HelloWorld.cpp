// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Runtime.h"

int main()
{
	Runtime runtime;
	runtime.Execute("trace(\"Hello\" + \", World\");");
	return 0;
}

//EOF
