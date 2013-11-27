// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#include "Om/Precompiled.h"
#include <boost/unordered_set.hpp>

OM_BEGIN

struct MarkingContextImpl
{
	// TODO: use MonotonicUnorderedSet<>
	typedef boost::unordered_set<Handle::Value> Handles;
	Handles handles;
};

boost::scoped_ptr<MarkingContextImpl> impl;

MarkingContext::MarkingContext(bool M)
	: impl(new MarkingContextImpl()), mark(M)
{
}

MarkingContext::~MarkingContext()
{
}

bool MarkingContext::Reset() const
{
	return reset;
}

bool MarkingContext::Mark() const
{
	return mark;
}

bool MarkingContext::IsMarked() const
{
	OM_NOT_IMPLEMENTED();
}

bool MarkingContext::Enter(Handle handle)
{
	if (impl->handles.find(handle.value) != impl->handles.end())
		return false;
	impl->handles.insert(handle.value);
	return true;
}

OM_END

//EOF
