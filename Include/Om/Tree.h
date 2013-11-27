// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include "Om/Pathname.h"

OM_BEGIN

struct Tree
{
	typedef std::vector<Object> Searchpaths;

	Object root;
	Searchpaths paths;

};

OM_END

//EOF
