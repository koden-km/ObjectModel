
#pragma once

#include <boost/noncopyable.hpp>

/// A structure derived from NonCopyable cannot be copied either by assignment
/// or copy-construction.
typedef boost::noncopyable NonCopyable;

//EOF
