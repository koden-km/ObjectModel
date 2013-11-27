// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

struct MarkingContextImpl;

/// used to avoid recursion when marking an object graph
struct MarkingContext : boost::noncopyable
{
	boost::scoped_ptr<MarkingContextImpl> impl;
	bool mark;
	bool reset;

	MarkingContext(bool mark);
	~MarkingContext();

	bool Reset() const;
	bool Mark() const;
	bool IsMarked() const;
	bool Enter(Handle handle);
};

struct IMarkable
{
	virtual void SetMarked(MarkingContext &) const = 0;
	//virtual bool IsMarked() const = 0;
};

OM_END

//EOF
