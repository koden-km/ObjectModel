// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

#include <map>

OM_BEGIN

/// a dictionary is a mapping from label to object
struct Dictionary
{
	typedef std::map<Label, Object> Objects;
	typedef Objects::const_iterator const_iterator;
	typedef Objects::value_type value_type;

//protected:
	Objects objects;

public:
	void Add(Label const &, Object const &);
	Object Find(Label const &) const;
	void Erase(Label const &);

	size_t size() const { return objects.size(); }
	bool empty() const { return objects.empty(); }
	const_iterator begin() const { return objects.begin(); }
	const_iterator end() const { return objects.end(); }

	Object &operator[](Label const &L) { return objects[L]; }
};

// specialised trace used by executor
void TraceDictionary(Object const &Q);

OM_END

//EOF
