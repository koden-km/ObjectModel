// (C) 2009 Christian.Schladetsch@Gmail.com.
// (C) 2009 Blue Lion Software Limited.

#pragma once

OM_BEGIN

#define OM_DEBUG_SYMBOLS

/// structure common to all stored values and references
struct StorageBase : Object, HasLabel
{
private:
	Dictionary dictionary;

	// TODO: use a bitfield
	bool reference;
	bool konst;
	mutable bool marked;
	mutable Handle parent_handle;

public:
	StorageBase(Object const &Q, bool ref = false, bool konst = false);
	~StorageBase();

#ifdef OM_DEBUG_SYMBOLS
	FileLocation file_location;
#endif

	bool HasParent() const;
	Object GetParent() const;
	Handle GetParentHandle() const { return parent_handle; }
	bool IsReference() const { return reference; }
	bool IsConst() const { return konst; }

	void Set(Label const &, Object &child);
	bool Has(Label const &) const;
	void Remove(Label const &);
	Object Get(Label const &) const;
	Object Get(Label const &);

	Dictionary const &GetDictionary() const { return dictionary; }

	template <class T>
	bool IsType() const
	{
		return klass->GetTypeNumber().value == Type::Traits<T>::TypeNumber.value;
	}

	template <class Fun>
	void ForEachChild(Fun &fun) const
	{
		foreach (Dictionary::Objects::value_type const &val, dictionary.objects)
		{
			fun(val.second.GetStorage());
		}
		ForEachProperty(fun);
	}
	template <class Fun>
	void ForEachChild(Fun &fun)
	{
		foreach (Dictionary::Objects::value_type &val, dictionary.objects)
		{
			fun(val.second.GetStorage());
		}
		ForEachProperty(fun);
	}
	template <class Fun>
	void ForEachProperty(Fun &fun) const
	{
		foreach (ClassBase::Properties::value_type const &iter, klass->GetProperties())
		{
			PropertyBase const *prop = iter.second;
			if (!prop->IsObject())
				continue;
			Object child = prop->Get(*this);
			if (!child)
				continue;
			fun(child.GetStorage());
		}
	}
	template <class Fun>
	void ForEachSubObject(Fun &fun)
	{
		foreach (Dictionary::value_type &val, dictionary.objects)
		{
			fun(val.first, val.second);
		}
		foreach (ClassBase::Properties::value_type const &iter, klass->GetProperties())
		{
			PropertyBase const *prop = iter.second;
			Object child = prop->Get(*this);
			if (!child)
				continue;
			fun(prop->GetLabel(), child);
		}
	}
	template <class Fun>
	void ForEachSubObject(Fun &fun) const
	{
		foreach (Dictionary::value_type const &val, dictionary.objects)
		{
			fun(val.first, val.second);
		}
		foreach (ClassBase::Properties::value_type const &iter, klass->GetProperties())
		{
			PropertyBase const *prop = iter.second;
			Object child = prop->Get(*this);
			if (!child)
				continue;
			fun(prop->GetLabel(), child);
		}
	}
	void SetMarked(MarkingContext &mark) const;
	bool IsMarked() const;

private:
	friend struct Object;
	template <class T>
	friend struct Class;
	void Attach(Object const &parent, Label const & label);
	void Detach(Object const &parent);
};

OM_END

//EOF

