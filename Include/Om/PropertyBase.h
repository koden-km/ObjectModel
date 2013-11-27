// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// structure common to all properties
struct PropertyBase : HasLabel
{
	PropertyBase() { }
	PropertyBase(Label const &L) : HasLabel(L) { }

	/// if the property is a C++ type, returns a system type initialised with its value,
	/// else it is an Object and it returns that
	virtual Object Get(Object &) const = 0;
	virtual Object Get(Object const &) const = 0;

	/// if the property is a C++ type, sets its value, else it Deref<T>'s it and sets that
	virtual void Set(Object &parent, Object &child) const = 0;
	virtual void Reset(Object &parent) const = 0;

	// return true if the property is an Object or a Pointer<> or a Reference<>
	virtual bool IsObject() const = 0;

	virtual void SetOwner(Object &, Label const &) { }
};

OM_END

//EOF
