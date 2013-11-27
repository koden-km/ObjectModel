// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// structure common to all reflected objects.
///
/// a reflected object has a Self pointer, which is analogous to `this`, but within
/// the context of the reflected object model
struct ReflectedBase : BaseBase//, IMarkable
{
	StorageBase *Self;

	/// invoked after Self has been set, and all properties created
	virtual void Construct() { }

	/// invoked after the object has been marked for deletion
	virtual void Destroy() { }

};

OM_END

//EOF
