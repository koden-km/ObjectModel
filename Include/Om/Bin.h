// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

/// provides functionality from the console and for scripts
struct Bin : Reflected<Bin>
{
	/// returns a list of child objects (children and properties) for a given object
	Object/*Pointer<List> */GetContents(Object const &) const;

	/// returns the methods exposed for a given object
	Object/*Pointer<List> */GetMethods(Object const &) const;

	/// returns a description of each method exposed for a given object
	Object/*Pointer<List> */GetMethodStrings(Object const &) const;
	
	/// change the current scope
	void ChangeContext(Object const &) const;

	/// prints a textual representation of an object to stdout
	String ToString(Object const &) const;

	/// debug-trace the full contents of an object
	void Trace(Object const &) const;

	/// debug-trace the full contents of an object
	void Print(Object const &) const;

	/// enable or disable the given debug stream
	void EnableStream(int N, bool) const;

	/// 'freeze' an object into a binary packet
	Object/*Pointer<BinaryStream>*/ Freeze(Object const &) const;

	// return the number of objects in the registry
	size_t NumObjects() const;

	// perform a full garbage collection
	void GarbageCollect() const;

	/// 'thaw' an object from a binary packet back into a live object
	Object Thaw(BinaryStream &) const;

	static void Register(Registry &);
};

OM_TRAITS(Bin);

OM_END

//EOF
