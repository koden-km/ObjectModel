#pragma once

#include <vector>
#include <string>
#include <sstream>

#include <boost/range.hpp>
#include <boost/array.hpp>
#include <boost/iterator/counting_iterator.hpp>

typedef std::string String;
typedef std::stringstream StringStream;


template <class T>
std::pair<boost::counting_iterator<T>, boost::counting_iterator<T> > range(T start, T end)
{
	typedef boost::counting_iterator<T> cit;
	return std::make_pair(cit(start), cit(end));
};

#include "Om/Utility/BaseBase.h"
#include "Om/Utility/Optional.h"
#include "Om/Utility/IToString.h"
#include "Om/Utility/FileLocation.h"
#include "Om/Utility/ExceptionBase.h"
#include "Om/Utility/StringFormat.h"

#define OM_TRACE_ENABLE

#include "Om/Debug/Trace/StreamBase.h"
#include "Om/Debug/Trace/Stream.h"
#include "Om/Debug/Trace/ITraceable.h"
#include "Om/Debug/Trace/Scoped.h"
#include "Om/Debug/Trace/Detail/Streamer.h"
#include "Om/Debug/Write.h"
#include "Om/Debug/Trace/Printer.h"
#include "Om/Debug/Macros.h"
#include "Om/Debug/Trace/Containers.h"

using Debug::detail::DerefPointers;

//EOF
